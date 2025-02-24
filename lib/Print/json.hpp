#ifndef _JSON_
#define _JSON_

#include <iostream>
#include <nlohmann/json.hpp>
#include <Parse/parser.hpp>
#include <Time/converters.hpp>

using json = nlohmann::json;

std::string GetTicketsPrice(json r);

void PrintPath(const json cur, const json response, const ApiInfo info, size_t& cnt);

#endif