#include "arg_parser.hpp"

namespace argument_parser {

std::queue<std::pair<std::string, size_t>> positional_queue;

Argument::Argument(ArgInfo& arg_info) : arg_info_(arg_info) {
    while (!positional_queue.empty()) {
        positional_queue.pop();
    }
}

Argument& Argument::MultiValue(int min_arg_count) {
    arg_info_.is_multi_value = true;
    arg_info_.min_arg_count = min_arg_count;
    return *this;
}

Argument& Argument::Positional() {
    arg_info_.is_positional = true;
    positional_queue.push({arg_info_.long_name, arg_info_.min_arg_count});
    return *this;
}

ArgParser::ArgParser() { parser_name_ = ""; }

ArgParser::ArgParser(const std::string& name) { parser_name_ = name; }

Argument ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto arg_string = std::make_unique<ArgString>();
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}


Argument ArgParser::AddStringArgument(const char short_name, const std::string& long_name, const std::string& description) {
    if (!short_name) {
        std::cerr << "Error: short argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_name == '-') {
        std::cerr << "Error: short argument cannot be named as '-'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_arg_map_.find(short_name) != short_arg_map_.end()) {
        std::cerr << "Error: second '" << short_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    short_arg_map_[short_name] = long_name;

    auto arg_string = std::make_unique<ArgString>();
    arg_string->short_name = short_name;
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

Argument ArgParser::AddUint239_tArgument(const std::string& long_name, const std::string& description) {
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto arg_string = std::make_unique<ArgUint239_t>();
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

Argument ArgParser::AddUint239_tArgument(const char short_name, const std::string& long_name, const std::string& description) {
    if (!short_name) {
        std::cerr << "Error: short argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_name == '-') {
        std::cerr << "Error: short argument cannot be named as '-'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_arg_map_.find(short_name) != short_arg_map_.end()) {
        std::cerr << "Error: second '" << short_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    short_arg_map_[short_name] = long_name;

    auto arg_string = std::make_unique<ArgUint239_t>();
    arg_string->short_name = short_name;
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

Argument ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto arg_string = std::make_unique<ArgInt>();
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

Argument ArgParser::AddIntArgument(const char short_name, const std::string& long_name, const std::string& description) {
    if (!short_name) {
        std::cerr << "Error: short argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_name == '-') {
        std::cerr << "Error: short argument cannot be named as '-'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_arg_map_.find(short_name) != short_arg_map_.end()) {
        std::cerr << "Error: second '" << short_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    short_arg_map_[short_name] = long_name;

    auto arg_string = std::make_unique<ArgInt>();
    arg_string->short_name = short_name;
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

Argument ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
    if (long_name.empty()) {
        std::cerr << "Error: long argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto arg_string = std::make_unique<ArgBool>();
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

Argument ArgParser::AddFlag(const char short_name, const std::string& long_name, const std::string& description) {
    if (!short_name) {
        std::cerr << "Error: short argument name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_name == '-') {
        std::cerr << "Error: short flag cannot be named as '-'" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (long_name.empty()) {
        std::cerr << "Error: long flag name cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (short_arg_map_.find(short_name) != short_arg_map_.end()) {
        std::cerr << "Error: second '" << short_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (arg_map_.find(long_name) != arg_map_.end()) {
        std::cerr << "Error: second '" << long_name << "' overrides already existing argument" << std::endl;
        exit(EXIT_FAILURE);
    }
    short_arg_map_[short_name] = long_name;

    auto arg_string = std::make_unique<ArgBool>();
    arg_string->short_name = short_name;
    arg_string->long_name = long_name;
    arg_string->description = description;

    arg_map_[long_name] = std::move(arg_string);

    return Argument(*arg_map_[long_name]);
}

void ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
    AddFlag(short_name, long_name, description);
    help_flag_ = long_name;
    help_short_flag_ = short_name;
}

void ArgParser::AddHelp(const std::string& long_name, const std::string& description) {
    AddFlag(long_name, description);
    help_flag_ = long_name;
}

bool ArgParser::Help() const {
    return help_flag_encountered_;
}

std::string ArgParser::HelpDescription() const {
    std::ostringstream out;
    out << "Usage: " << parser_name_ << " [options]\n\n";
    out << "Options:\n";

    for (const auto& [name, info_ptr] : arg_map_) {
        // Разыменовываем умный указатель
        const auto& info = *info_ptr;

        std::string short_name;
        if (info.short_name) {
            short_name = std::string(1, info.short_name);
        }

        out << "  ";
        if (!short_name.empty()) {
            out << "-" << short_name << ", ";
        }
        out << "--" << name;

        if (info.GetTypeName() == "String") {
            out << "=<string>";
        } else if (info.GetTypeName() == "Int") {
            out << "=<int>";
        } else if (info.GetTypeName() == "Bool") {
            out << "=<bool>";
        } else if (info.GetTypeName() == "Uint239_t") {
            out << "=<uint239_t>";
        }

        out << "\t\t" << info.description << "\n";
    }

    return out.str();
}


std::string ArgParser::GetStringValue(const std::string& str) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto& arg = static_cast<ArgString&>(*arg_map_[str]);
    return arg.value;
}


std::string ArgParser::GetStringValue(const std::string& str, int n) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto& arg = static_cast<ArgString&>(*arg_map_[str]);

    if (arg.values.empty()) {
        std::cerr << "Error: values for '" << str << "' are empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    return arg.values[n];
}


uint239_t ArgParser::GetUint239_tValue(const std::string& str) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto& arg = static_cast<ArgUint239_t&>(*arg_map_[str]);
    return arg.value;
}

uint239_t ArgParser::GetUint239_tValue(const std::string& str, int n) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto& arg = static_cast<ArgUint239_t&>(*arg_map_[str]);

    if (arg.values.empty()) {
        std::cerr << "Error: values for '" << str << "' are empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    return arg.values[n];
}

int ArgParser::GetIntValue(const std::string& str) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto& arg = static_cast<ArgInt&>(*arg_map_[str]);
    return arg.value;
}

int ArgParser::GetIntValue(const std::string& str, int n) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto& arg = static_cast<ArgInt&>(*arg_map_[str]);

    if (arg.values.empty()) {
        std::cerr << "Error: values for '" << str << "' are empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    return arg.values[n];
}

bool ArgParser::GetFlag(const std::string& str) {
    if (arg_map_.find(str) == arg_map_.end()) {
        std::cerr << "Error: unknown argument '" << str << "'!" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto& arg = static_cast<ArgBool&>(*arg_map_[str]);

    return arg.value;
}

bool ArgParser::Parse(int argc, char* argv[]) {
    if (argc < 1) {
        return false; // Wrong amount of arguments. Minimum is 1, i.e. name of the program
    }
    std::string program_path = argv[0]; // Default it is something like: "C:/users/user/folder/app.exe".
    std::string program_name = std::filesystem::path(program_path).filename().string();

    std::vector<std::string> args(argc); // Vector containing all values of 'argv' (program path in argv[0] is reduced down to the name of the program).
    args[0] = program_name;
    for (int i = 1; i < argc; ++i) { // Filling vector 'args' with values of 'argv'.
        args[i] = argv[i];
    }

    return Parse(args);
}

bool ArgParser::Parse(const std::string& str) {
    if (str.empty()) {
        return false; // We do not need to parse empty string.
    }

    std::istringstream iss(str);
    std::vector<std::string> splited_str{std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
    return Parse(splited_str);
}

bool ArgParser::Parse(const std::vector<std::string>& str) {
    if (str.empty()) {
        return false; // We do not need to parse empty vector;
    }

    std::string_view program_name = str[0]; // First element must be program name.

    if (program_name.empty()) {
        return false;
    }
    if (program_name[0] == '-') {
        std::cerr << "Warning: program name '" << program_name << "' starts with hyphen '-'" << std::endl;
    }
    program_name_ = program_name;

    for (int i = 1; i < str.size(); ++i) {
        const std::string_view arg = str[i];
        if (StartsWith(arg, "--") && arg.size() >= 3) {
            if (!help_flag_.empty() && arg == "--" + help_flag_) {
                help_flag_encountered_ = true;
                continue;
            }
            if (!ParseLongForm(arg, str, i)) {
                return false;
            }
        } else if (StartsWith(arg, '-') && arg.size() >= 2) {
            if (arg == "-" + std::string(1, help_short_flag_)) {
                help_flag_encountered_ = true;
                continue;
            }
            if (!ParseShortForm(arg, str, i)) {
                return false;
            }
        } else if (!(StartsWith(arg, '-')) && !(arg.empty())) {
            if (!ParsePositional(arg)) {
                return false;
            }
        } else if (arg.size() == 1 && arg[0] == '-') {
            std::cerr << "Error: missing argument for '-'" << std::endl;
            return false;
        } else {
            return false;
        }
    }

    // Check for all multivalue arguments to be completed.
    for (auto& i : arg_map_) {
        if (!i.second->is_multi_value) {
            continue;
        }
        if (i.second->GetTypeName() == "Int") {
            auto& arg1 = static_cast<ArgInt&>(*i.second);
            if (arg1.p_values && str.size() != 1 && arg1.p_values->size() < i.second->min_arg_count) {
                return false;
            }
        } else if (i.second->GetTypeName() == "String") {
            auto& arg2 = static_cast<ArgString&>(*i.second);
            if (arg2.p_values && arg2.p_values->size() < i.second->min_arg_count) {
                return false;
            }
        } else if (i.second->GetTypeName() == "Uint239_t") {
            auto& arg3 = static_cast<ArgUint239_t&>(*i.second);
            if (arg3.p_values && arg3.p_values->size() < i.second->min_arg_count) {
                return false;
            }
        }
    }



    return true;
}

bool ArgParser::ParseLongForm(const std::string_view& arg, 
                   const std::vector<std::string>& str, 
                   int& pos) {
        if (arg.size() < 3 || arg.substr(0, 2) != "--") {
            return false;  // Argument must start with '--' and be at least 3 characters.
        }

        // Extract the long argument name and value (if provided with '=').
        std::string long_name;
        std::string value;

        size_t equal_pos = arg.find('=');
        if (equal_pos != std::string::npos) {
            long_name = std::string(arg.substr(2, equal_pos - 2));  // Part before '='.
            value = std::string(arg.substr(equal_pos + 1));         // Part after '='.
        } else {
            long_name = std::string(arg.substr(2));  // No '=' means entire part is the name.
        }

        if (arg_map_.find(long_name) == arg_map_.end()) {
            std::cerr << "Error: unknown long argument --" << long_name
                      << std::endl;
            return false;  // Argument name not found in the map.
        }

        ArgInfo& info = *arg_map_[long_name];

        if (info.GetTypeName() == "Bool") {
            if (!value.empty()) {
                std::cerr << "Error: unexpected value for flag --" 
                          << long_name << std::endl;
                return false;  // Flags should not have associated values.
            }
            auto& ij = static_cast<ArgBool&>(info);
            ij.value = true;
            if (ij.p_value) {
                *ij.p_value = true;
            }
            return true;  // Successfully processed flag.
        }

        // Non-flag arguments require a value.
        if (info.GetTypeName() == "String" || info.GetTypeName() == "Int") {
            if (value.empty()) {
                // If no value was provided with '=', check the next argument.
                if (pos + 1 < str.size() && str[pos + 1][0] != '-') {
                    value = str[++pos];
                } else {
                    std::cerr << "Error: missing value for --" << long_name << "\n";
                    return false;  // Value is required but not found.
                }
            }

            if (info.GetTypeName() == "Int") {
                char* end;
                long int_value = std::strtol(value.c_str(), &end, 10);
                if (*end != '\0') {
                    std::cerr << "Error: invalid integer value for --" 
                            << long_name << ": " << value << "\n";
                    return false;  // Value is not a valid integer.
                }
            }

            StoreArgumentValue(info, value);  // Store the value.
            return true;  // Successfully processed argument with value.
        }

        if (info.GetTypeName() == "Uint239_t") {
            if (value.empty()) {
                // If no value was provided with '=', check the next argument.
                if (pos + 1 < str.size() && str[pos + 1][0] != '-') {
                    value = str[++pos];
                } else {
                    std::cerr << "Error: missing value for --" << long_name << "\n";
                    return false;  // Value is required but not found.
                }
            }

            StoreArgumentValue(info, value);  // Store the value.
            return true;  // Successfully processed argument with value.
        }

        std::cerr << "Error: unsupported argument type for --" << long_name << "\n";
        return false;  // Unsupported argument type.
    }


bool ArgParser::ParseShortForm(const std::string_view& arg, 
                    const std::vector<std::string>& str, 
                    int& pos) {
        if (arg[0] != '-' || arg[1] == '-') {
            return false;  // Return false if the argument is not a short form.
        }

        for (size_t i = 1; i < arg.size(); ++i) {
            char short_name = arg[i];

            if (short_arg_map_.find(short_name) == short_arg_map_.end()) {
                std::cerr << "Error: unknown short argument -" 
                          << short_name << "\n";
                return false;  // Skip to the next argument if unknown.
            }

            const std::string& long_name = short_arg_map_[short_name];
            ArgInfo& info = *arg_map_[long_name];

            if (info.GetTypeName() == "Bool") {
                auto& ij = static_cast<ArgBool&>(info);
                ij.value = true;
                if (ij.p_value) {
                    *ij.p_value = true;
                }
                continue;  // No value expected for flag arguments.
            }

            if (info.GetTypeName() == "String" || info.GetTypeName() == "Int") {
                // Handle argument with a value.
                if (arg.size() > 2 && arg[i + 1] == '=') {
                    std::string value = std::string(arg.substr(i + 2));
                    if (value.empty()) {
                        std::cerr << "Error: missing value for argument -" 
                                << short_name << "\n";
                        return false;  // Return false if no value after '='.
                    }

                    if (info.GetTypeName() == "Int") {
                        char* end;
                        long int_value = std::strtol(value.c_str(), &end, 10);
                        if (*end != '\0') {
                            std::cerr << "Error: invalid integer value for -" 
                                    << short_name << ": " << value << "\n";
                            return false;  // Return false if integer conversion fails.
                        }
                    }

                    StoreArgumentValue(info, value);  // Store the value.
                    break;  // Break once the value is processed.
                }

                if (pos + 1 < str.size() && str[pos + 1][0] != '-') {
                    // Handle value passed as the next argument.
                    std::string value = str[++pos];
                    if (value.empty()) {
                        std::cerr << "Error: missing value for argument -" 
                                << short_name << "\n";
                        return false;  // Return false if value is empty.
                    }

                    if (info.GetTypeName() == "Int") {
                        char* end;
                        long int_value = std::strtol(value.c_str(), &end, 10);
                        if (*end != '\0') {
                            std::cerr << "Error: invalid integer value for -" 
                                    << short_name << ": " << value << "\n";
                            return false;  // Return false if integer conversion fails.
                        }
                    }

                    StoreArgumentValue(info, value);  // Store the value.
                    break;  // Break once the value is processed.
                }

                std::cerr << "Error: missing or invalid value for -" 
                        << short_name << "\n";
                return false;  // Return false if no valid value is found.
            }

            if (info.GetTypeName() == "Uint239_t") {
                if (arg.size() > 2 && arg[i + 1] == '=') {
                    std::string value = std::string(arg.substr(i + 2));
                    if (value.empty()) {
                        std::cerr << "Error: missing value for argument -" 
                                << short_name << "\n";
                        return false;  // Return false if no value after '='.
                    }

                    StoreArgumentValue(info, value);  // Store the value.
                    break;  // Break once the value is processed.
                }

                if (pos + 1 < str.size() && str[pos + 1][0] != '-') {
                    // Handle value passed as the next argument.
                    std::string value = str[++pos];
                    if (value.empty()) {
                        std::cerr << "Error: missing value for argument -" 
                                << short_name << "\n";
                        return false;  // Return false if value is empty.
                    }

                    StoreArgumentValue(info, value);  // Store the value.
                    break;  // Break once the value is processed.
                }

                std::cerr << "Error: missing or invalid value for -" 
                        << short_name << "\n";
                return false;  // Return false if no valid value is found.
            }

            std::cerr << "Error: unsupported argument type for -" 
                    << short_name << "\n";
            return false;  // Return false if the argument type is unsupported.
        }

        return true;  // Return true if all arguments are successfully parsed.
    }

bool ArgParser::ParsePositional(const std::string_view& arg) {
    // Check if there are any expected positional arguments
    if (positional_queue.empty()) {
        std::cerr << "Error: no positional arguments expected, but got: "
                << arg << "\n";
        return false;
    }

    // Extract the current positional argument from the queue
    auto& [name, min_arg_count] = positional_queue.front(); 
    // Get the name and minimum required argument count
    auto& info = *arg_map_[name]; // Retrieve information about the argument
    // Ensure the type of the argument is supported (String or Int)
    if (info.GetTypeName() != "String" && 
        info.GetTypeName() != "Int" && 
        info.GetTypeName() != "Uint239_t") {
        std::cerr << "Error: unsupported type for positional argument: " 
                << name << std::endl;
        return false;
    }

    // Handle single-value positional arguments
    if (!info.is_multi_value) {
        if (info.GetTypeName() == "Int") {
            char* end;
            long int_value = std::strtol(arg.data(), &end, 10);
            if (*end != '\0') {
                std::cerr << "Error: invalid integer value for positional "
                        << "argument " << name << ": " << arg << "\n";
                return false;
            }
            auto& ij = static_cast<ArgInt&>(info);
            ij.value = static_cast<int>(int_value);
            if (ij.p_value) {
                *ij.p_value = static_cast<int>(int_value);
            }
        } else if (info.GetTypeName() == "String") {
            auto& ij = static_cast<ArgString&>(info);
            ij.value = std::string(arg);
            if (ij.p_value) {
                *ij.p_value = std::string(arg);
            }
        } else if (info.GetTypeName() == "Uint239_t") {
            auto& ij = static_cast<ArgUint239_t&>(info);
            ij.value = FromString(std::string(arg).c_str(), 0);
            if (ij.p_value) {
                *ij.p_value = FromString(std::string(arg).c_str(), 0);
            }
        }

        // Remove the single argument from the queue
        positional_queue.pop();
        return true;
    }

    // Parse values for multi-value positional arguments
    if (info.GetTypeName() == "Int") {
        char* end;
        long int_value = std::strtol(arg.data(), &end, 10);
        if (*end != '\0') {
            std::cerr << "Error: invalid integer value for positional "
                    << "argument " << name << ": " << arg << "\n";
            return false;
        }
        auto& ij = static_cast<ArgInt&>(info);
        ij.values.push_back(static_cast<int>(int_value));
        if (ij.p_values) {
            ij.p_values->push_back(static_cast<int>(int_value));
        }
    } else if (info.GetTypeName() == "String") {
        auto& ij = static_cast<ArgString&>(info);
        ij.values.push_back(std::string(arg));
        if (ij.p_values) {
            ij.p_values->push_back(std::string(arg));
        }
    } else if (info.GetTypeName() == "Uint239_t") {
        auto& ij = static_cast<ArgUint239_t&>(info);
        ij.values.push_back(FromString(std::string(arg).c_str(), 0));
        if (ij.p_values) {
            ij.p_values->push_back(FromString(std::string(arg).c_str(), 
                                                        0));
        }
    }
    // If it is the last positional argment we can return without changes.
    if (positional_queue.size() == 1) {
        return true;
    }

    // Decrease the counter for the minimum required arguments
    --min_arg_count;

    // Remove the argument from the queue if its count reaches 0
    if (min_arg_count <= 0) {
        positional_queue.pop();
    }

    return true;
}

void ArgParser::StoreArgumentValue(ArgInfo& info, const std::string& value) {
    if (info.is_multi_value) {
        if (info.GetTypeName() == "Int") {
            int ivalue = std::stoi(value);
            auto& ij = static_cast<ArgInt&>(info);
            ij.values.push_back(static_cast<int>(ivalue));
            if (ij.p_values) {
                ij.p_values->push_back(static_cast<int>(ivalue));
            }
        } else if (info.GetTypeName() == "String") {
            auto& ij = static_cast<ArgString&>(info);
            ij.values.push_back(std::string(value));
            if (ij.p_values) {
                ij.p_values->push_back(std::string(value));
            }
        } else if (info.GetTypeName() == "Uint239_t") {
            auto& ij = static_cast<ArgUint239_t&>(info);
            ij.values.push_back(FromString(std::string(value).c_str(), 0));
            if (ij.p_values) {
                ij.p_values->push_back(FromString(std::string(value).c_str(), 
                                                            0));
            }
        }
    } else {
        if (info.GetTypeName() == "Int") {
            int ivalue = std::stoi(value);
            auto& ij = static_cast<ArgInt&>(info);
            ij.value = static_cast<int>(ivalue);
            if (ij.p_value) {
                *ij.p_value = static_cast<int>(ivalue);
            }
        } else if (info.GetTypeName() == "String") {
            auto& ij = static_cast<ArgString&>(info);
            ij.value = std::string(value);
            if (ij.p_value) {
                *ij.p_value = std::string(value);
            }
        } else if (info.GetTypeName() == "Uint239_t") {
            auto& ij = static_cast<ArgUint239_t&>(info);
            ij.value = FromString(std::string(value).c_str(), 0);
            if (ij.p_value) {
                *ij.p_value = FromString(std::string(value).c_str(), 0);
            }
        }
    }
}

bool ArgParser::StartsWith(const std::string_view& str1, 
                           const std::string_view& str2) {
    if (str2.empty()) {
        return true;
    }
    
    if (str1.empty()) {
        return false;
    }

    if (str2.size() > str1.size()) {
        return false;
    }

    for (size_t i = 0; i < str2.size(); ++i) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    
    return true;
}
bool ArgParser::StartsWith(const std::string_view& str, char ch) {
    return !str.empty() && str[0] == ch;
}

bool ArgParser::StartsWith(char ch1, char ch2) {
    return ch1 == ch2;
}

} // end namespace argument_parser