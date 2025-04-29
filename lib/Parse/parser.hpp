#ifndef _PARSER_
#define _PARSER_

#include <string>
#include <iostream>
#include <ArgParser/arg_parser.hpp>


using ArgParser = argument_parser::ArgParser;

struct ApiInfo {
    std::string date;
    std::string from;
    std::string to;
    std::string api_key;
    std::string cash_file_name;
    int max_transfer_cnt;
};

void FillCmdLineParser(ArgParser& parser);

void FillApiInfo(ApiInfo& info, ArgParser& parser);

bool ParseCmdLine(ArgParser& parser, int argc, char* argv[]);

#endif