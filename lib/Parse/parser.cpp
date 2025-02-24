#include "parser.hpp"

void FillCmdLineParser(ArgParser& parser) {
    parser.AddStringArgument('d', "date",    "Date of departure in YYYY:MM:DD format");
    std::string api_key = std::getenv("YANDEX_RASP_API_KEY");
    parser.AddStringArgument('a', "api-key", "Sets api key").Default(api_key);
    parser.AddStringArgument('s', "spb", "Sets departure city").Default("c2");
    parser.AddStringArgument('p', "pskov", "Sets arrival city").Default("c25");

    parser.AddIntArgument("max-transfer", "Sets max amount of possible transfers").Default(1);
    parser.AddIntArgument("max-time", "Sets max amount of travel hours").Default(100000);

    parser.AddFlag("from-spb-to-pskov", "Sets direction");
    parser.AddFlag("from-pskov-to-spb", "Sets direction");

    parser.AddHelp('h', "help");
}

void FillApiInfo(ApiInfo& info, ArgParser& parser) {
    if (parser.GetFlag("from-spb-to-pskov")) {
        info.from = parser.GetStringValue("spb");
        info.to = parser.GetStringValue("pskov");
    } else {
        info.from = parser.GetStringValue("pskov");
        info.to = parser.GetStringValue("spb");
    }
    info.api_key = parser.GetStringValue("api-key");
    info.date = parser.GetStringValue("date");
    info.cash_file_name = std::format("{}-{}-{}.txt", info.date, info.from, info.to);
    info.max_transfer_cnt = parser.GetIntValue("max-transfer");
    info.max_travel_time = parser.GetIntValue("max-time");
}

bool ParseCmdLine(ArgParser& parser, int argc, char* argv[]) {
    if (!parser.Parse(argc, argv)) {
        std::cerr << "Error: wrong argument!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return false;
    }


    if (!(parser.GetFlag("from-spb-to-pskov") 
        || parser.GetFlag("from-pskov-to-spb"))) {
        std::cerr << "Error: please enter direction!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return false;
    }

    
    if (parser.GetFlag("from-spb-to-pskov") 
        && parser.GetFlag("from-pskov-to-spb")) {
        std::cerr << "Error: You can only choose one direction" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return false;
    }
    

    if (parser.GetStringValue("date") == "") {
        std::cerr << "Error: please enter a date!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return false;
    } else if (parser.GetStringValue("date").size() != 10 
               && parser.GetStringValue("date")[4] != ':') {
        std::cerr << "Error: wrong fromat!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return false;
    }
    

    if (parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
    }


    return true;
}