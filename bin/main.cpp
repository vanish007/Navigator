#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <ArgParser/arg_parser.hpp>
#include <Parse/parser.hpp>
#include <Print/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    ArgParser parser("spb-pskov");
    FillCmdLineParser(parser);
    if (!ParseCmdLine(parser, argc, argv)) {
        return EXIT_FAILURE;
    }    
    

    ApiInfo info;
    FillApiInfo(info, parser);
    

    json response;
    const fs::path cash("cash");
    if (!fs::exists(cash/info.cash_file_name)) {
        cpr::Url url = cpr::Url{"https://api.rasp.yandex.net/v3.0/search/"};
        cpr::Parameters parameters = cpr::Parameters{{"apikey", info.api_key},
                                                     {"format", "json"},
                                                     {"from", info.from},
                                                     {"to", info.to},
                                                     {"date", info.date},
                                                     {"transfers", "true"}};
        cpr::Response temp = cpr::Get(url, parameters);
        response = json::parse(temp.text);
        
        std::ofstream fout;
        fs::create_directory(cash);
        fout.open(cash/info.cash_file_name);
        fout << response.dump(4);
        fout.close();
    } else {
        std::ifstream fin;
        fin.open(cash/info.cash_file_name);
        response = json::parse(fin);
    }

    if (response.find("error") != response.end()) {
        std::cout << "Error:" << response["error"]["text"];
        return EXIT_FAILURE;
    }


    size_t cnt = 1;
    for (json::iterator it = response["segments"].begin();
         it != response["segments"].end(); 
         ++it, ++cnt) {
        json cur = *it;
        PrintPath(cur, response, info, cnt);
    }
}