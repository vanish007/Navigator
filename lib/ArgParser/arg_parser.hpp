#ifndef _ARH_PARSER_
#define _ARH_PARSER_

#include <iostream>             
#include <unordered_map>        
#include <queue>              
#include <vector>              
#include <string>               
#include <iterator>             
#include <filesystem>          
#include <cstdint>              
#include <sstream>              
#include "../Number/number.hpp"           // For 'uint239_t'.
#include <typeinfo>

namespace argument_parser {

extern std::queue<std::pair<std::string, size_t>> positional_queue; // Queue for keeping track of the positional arguments.

struct ArgInfo{
    char short_name = {};
    std::string long_name = "";
    std::string description = "-";

    bool is_positional = false;
    bool is_multi_value = false;
    int min_arg_count = 0;

    virtual std::string GetTypeName() const = 0;
    virtual ~ArgInfo() = default;
};

struct ArgInt : ArgInfo {
    int value = {};
    std::vector<int> values = {};
    int* p_value = nullptr;
    std::vector<int>* p_values = nullptr;

    std::string GetTypeName() const override {
        return "Int";
    }

    ArgInt() {}
};

struct ArgString : ArgInfo {
    std::string value = {};
    std::vector<std::string> values = {};
    std::string* p_value = nullptr;
    std::vector<std::string>* p_values = nullptr;

    std::string GetTypeName() const override {
        return "String";
    }

    ArgString() {}
};

struct ArgBool : ArgInfo {
    bool value = false;
    bool* p_value= nullptr;
    std::vector<bool>* p_values = nullptr;

    std::string GetTypeName() const override {
        return "Bool";
    }

    ArgBool() {}
};

struct ArgUint239_t : ArgInfo {
    uint239_t value = {};
    std::vector<uint239_t> values{};
    uint239_t* p_value = nullptr;
    std::vector<uint239_t>* p_values = nullptr;

    std::string GetTypeName() const override {
        return "Uint239_t";
    }

    ArgUint239_t() {}
};

class Argument{
public:
    Argument(ArgInfo& arg_info);

    Argument& MultiValue(int min_arg_count = 0);
    Argument& Positional();

    template <typename T>
    Argument& Default(const T& default_value) {
        if constexpr (std::is_same_v<T, int>) {
            auto& int_info = static_cast<ArgInt&>(arg_info_);
            int_info.value = default_value;
            int_info.values.push_back(default_value);
        } else if constexpr (std::is_same_v<T, bool>) {
            auto& bool_info = static_cast<ArgBool&>(arg_info_);
            bool_info.value = default_value;
        } else if constexpr (std::is_same_v<T, uint239_t>) {
            auto& uint239_t_info = static_cast<ArgUint239_t&>(arg_info_);
            uint239_t_info.value = default_value;
            uint239_t_info.values.push_back(default_value);
        } else if constexpr (std::is_same_v<T, std::string> || std::is_array_v<T>) {
            auto& string_info = static_cast<ArgString&>(arg_info_);
            string_info.value = std::string(default_value); // Cast to string, cause of 'const char[7]' and others read like cstring, nut just regular string.
            string_info.values.push_back(string_info.value);
        } else {
            std::cerr << "Unsupported type in Default" << std::endl;
            exit(EXIT_FAILURE);
        }
        return *this;
    }
    
    Argument& Default(bool val) {
        auto& bool_info = static_cast<ArgBool&>(arg_info_);
        bool_info.value = val;
        return *this;
    } // Default

    template <typename T>
    void StoreValue(T& value) {
        if constexpr (std::is_same_v<T, int>) {
            auto& int_info = static_cast<ArgInt&>(arg_info_);
            int_info.p_value = &value;
        } else if constexpr (std::is_same_v<T, std::string>) {
            auto& string_info = static_cast<ArgString&>(arg_info_);
            string_info.p_value = &value;
        } else if constexpr (std::is_same_v<T, bool>) {
            auto& bool_info = static_cast<ArgBool&>(arg_info_);
            bool_info.p_value = &value;
        } else if constexpr (std::is_same_v<T, uint239_t>) {
            auto& uint239_t_info = static_cast<ArgUint239_t&>(arg_info_);
            uint239_t_info.value = &value;
        } else {
            std::cerr << "Unsupported type!" << std::endl;
        }
    } // StoreValue

    template <typename T>
    void StoreValues(T& value) {
        if constexpr (std::is_same_v<T, std::vector<int>>) {
            auto& int_info = static_cast<ArgInt&>(arg_info_);
            int_info.p_values = &value;
        } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
            auto& string_info = static_cast<ArgString&>(arg_info_);
            string_info.p_values = &value;
        } else if constexpr (std::is_same_v<T, std::vector<uint239_t>>) {
            auto& uint239_t_info = static_cast<ArgUint239_t&>(arg_info_);
            uint239_t_info.p_values = &value;
        } else {
            std::cerr << "Unsupported type!" << std::endl;
        }
    } // StoreValues

private:
    ArgInfo& arg_info_;
};

class ArgParser {
public:
    ArgParser();
    ArgParser(const std::string& name);

    Argument AddStringArgument(const std::string& long_name, const std::string& description = "-");
    Argument AddStringArgument(const char short_name, const std::string& long_name, const std::string& description = "-");

    Argument AddUint239_tArgument(const std::string& long_name, const std::string& description = "-");
    Argument AddUint239_tArgument(const char short_name, const std::string& long_name, const std::string& description = "-");

    Argument AddIntArgument(const std::string& long_name, const std::string& description = "-");
    Argument AddIntArgument(const char short_name, const std::string& long_name, const std::string& description = "-");

    Argument AddFlag(const std::string& long_name, const std::string& description = "-");
    Argument AddFlag(const char short_name, const std::string& long_name, const std::string& description = "-");

    void AddHelp(const char short_name, const std::string& long_name, const std::string& description = "Displays all available commands");
    void AddHelp(const std::string& long_name, const std::string& description = "Displays all available commands");

    bool Help() const;
    std::string HelpDescription() const;

    std::string GetStringValue(const std::string& str);
    std::string GetStringValue(const std::string& str, int n);

    uint239_t GetUint239_tValue(const std::string& str);
    uint239_t GetUint239_tValue(const std::string& str, int n);

    int GetIntValue(const std::string& str);
    int GetIntValue(const std::string& str, int n);

    bool GetFlag(const std::string& str);

    bool Parse(int argc, char* argv[]);
    bool Parse(const std::string& str);
    bool Parse(const std::vector<std::string>& str);
private:
    // Info variables.
    std::string parser_name_;
    std::string program_name_;

    // Help variables.
    bool help_flag_encountered_ = false;    // To track if the help flag was encountered.
    std::string help_flag_{};               // Custom long help flag name (e.g., "--help").
    char help_short_flag_{};                // Custom short help flag name (e.g., "-h").

    // Storage variables.
    std::unordered_map<char, std::string> short_arg_map_;    // Map that takes short arg name and returns its full name.
    std::unordered_map<std::string, std::unique_ptr<ArgInfo>> arg_map_;     // Map that takes argument name and returns its info.
    
    bool ParseLongForm(const std::string_view& arg, 
                   const std::vector<std::string>& str, 
                   int& pos);


    bool ParseShortForm(const std::string_view& arg, 
                    const std::vector<std::string>& str, 
                    int& pos);
    /**
     * This function 'ParsePositionalArguments' assigns positional 
     * arguments to the arguments marked as positional via '.Positional()'. 
     * It ensures arguments are assigned in the user-defined order and 
     * validates the count based on '.MultiValue()' value.
     */
    bool ParsePositional(const std::string_view& arg);

    void StoreArgumentValue(ArgInfo& info, const std::string& value);

    // Returns true if 'str2' is a correct prefix of 'str1' and false otherwise.
    bool StartsWith(const std::string_view& str1, const std::string_view& str2);

    // Returns true if 'ch' is a first character of 'str' and false otherwise.
    bool StartsWith(const std::string_view& str, char ch);

    // Returns true if 'ch1' is equal to 'ch2' and false otherwise.
    bool StartsWith(char ch1, char ch2);
}; // class ArgParser

} // namespace argument_parser

#endif