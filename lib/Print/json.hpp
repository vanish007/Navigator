#ifndef _JSON_
#define _JSON_

#include <iostream>
#include <nlohmann/json.hpp>
#include <Parse/parser.hpp>
#include <Time/converters.hpp>
#include <ftxui/dom/elements.hpp>  
#include <ftxui/dom/table.hpp>      
#include <ftxui/screen/screen.hpp>
#include "ftxui/dom/node.hpp"  
#include "ftxui/screen/color.hpp"

using json = nlohmann::json;
using namespace ftxui;

struct PrintInfo {
    std::string path;
    std::string name;
    std::string transfer_duration;
    std::string transport_type;
    std::string departure;
    std::string arrival;
    std::string total_time;
    std::string ticket_price;
};

std::string GetTicketsPrice(json r);

PrintInfo GetPrintPathInfo(json cur, json response, const ApiInfo api_info, size_t& cnt);

void PrintPath(const json cur, const json response, const ApiInfo info, size_t& cnt);


#endif