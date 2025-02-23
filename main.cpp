#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "ArgParser/arg_parser.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using ArgParser = argument_parser::ArgParser;

struct Info {
    std::string date;
    std::string from;
    std::string to;
    std::string api_key;
    std::string cash_file_name;
};

std::string TimestampToDate(long int seconds)
{
    std::string ans = "";
 
    int daysOfMonth[] = { 31, 28, 31, 30, 31, 30,
                          31, 31, 30, 31, 30, 31 };
 
    long int currYear, daysTillNow, extraTime, extraDays,
        index, date, month, hours, minutes, secondss,
        flag = 0;
 
    daysTillNow = seconds / (24 * 60 * 60);
    extraTime = seconds % (24 * 60 * 60);
    currYear = 1970;
 
    while (true) {
        if (currYear % 400 == 0
            || (currYear % 4 == 0 && currYear % 100 != 0)) {
            if (daysTillNow < 366) {
                break;
            }
            daysTillNow -= 366;
        }
        else {
            if (daysTillNow < 365) {
                break;
            }
            daysTillNow -= 365;
        }
        currYear += 1;
    }
    // Updating extradays because it
    // will give days till previous day
    // and we have include current day
    extraDays = daysTillNow + 1;
 
    if (currYear % 400 == 0
        || (currYear % 4 == 0 && currYear % 100 != 0))
        flag = 1;
 
    // Calculating MONTH and DATE
    month = 0, index = 0;
    if (flag == 1) {
        while (true) {
 
            if (index == 1) {
                if (extraDays - 29 < 0)
                    break;
                month += 1;
                extraDays -= 29;
            }
            else {
                if (extraDays - daysOfMonth[index] < 0) {
                    break;
                }
                month += 1;
                extraDays -= daysOfMonth[index];
            }
            index += 1;
        }
    }
    else {
        while (true) {
 
            if (extraDays - daysOfMonth[index] < 0) {
                break;
            }
            month += 1;
            extraDays -= daysOfMonth[index];
            index += 1;
        }
    }
 
    // Current Month
    if (extraDays > 0) {
        month += 1;
        date = extraDays;
    }
    else {
        if (month == 2 && flag == 1)
            date = 29;
        else {
            date = daysOfMonth[month - 1];
        }
    }
 
    // Calculating HH:MM:YYYY
    hours = extraTime / 3600;
    minutes = (extraTime % 3600) / 60;
    secondss = (extraTime % 3600) % 60;
 
    //replace with std::format
    ans += (std::string("0", 2-std::to_string(hours).size()) + std::to_string(hours));
    ans += ":";
    ans += (std::string("0", 2-std::to_string(minutes).size()) + std::to_string(minutes));
    ans += ":";
    ans += (std::string("0", 2-std::to_string(secondss).size()) + std::to_string(secondss));
    ans += " ";
    ans += std::to_string(date);
    ans += "/";
    ans += std::to_string(month);
    ans += "/";
    ans += std::to_string(currYear);
    // Return the time
    return ans;
}

std::time_t Iso8601ToTimestamp(const std::string& iso8601) {
    std::tm tm = {};
    std::istringstream ss(iso8601);

    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    int offsetHours = 0, offsetMinutes = 0;
    char sign;
    if (ss >> sign && (sign == '+' || sign == '-')) {
        ss >> std::setw(2) >> offsetHours;
        ss.ignore(1);
        ss >> std::setw(2) >> offsetMinutes;
        
        if (sign == '+') {
            offsetHours = -offsetHours;
            offsetMinutes = -offsetMinutes;
        }
    }

    std::time_t timeSinceEpoch = std::mktime(&tm);
    
    timeSinceEpoch += (6+offsetHours) * 3600 + offsetMinutes * 60;

    return timeSinceEpoch;
}

std::string SecondsToTime(long long seconds) {
    //what if the path will take longer than a day
    std::string ans = "";

    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secondss = (seconds % 3600) % 60;

    ans += std::to_string(hours) + " hours";
    ans += " and ";
    ans += std::to_string(minutes) + " minutes";
 
    return ans;
}

std::string GetTicketsPrice(json r) {
    for (json::iterator it = r.begin(); it != r.end(); ++it) {
        json cur = *it;
        return cur["price"]["whole"].dump(0) + " RUB";
    }
    return "-";
}

int main(int argc, char* argv[]) {
    const short kTranseferCnt = 1;
    std::setlocale(0, "");
    ArgParser parser("spb-pskov");
    parser.AddStringArgument('d', "date", "Date of departure in YYYY:MM:DD format");
    parser.AddStringArgument('a', "api-key", "Sets api key");
    parser.AddFlag('s', "sort", "Sorts stations by time and price (By default results are not sorted in any way)");
    parser.AddFlag("from-spb-to-pskov", "Sets direction");
    parser.AddFlag("from-pskov-to-spb", "Sets direction");
    parser.AddHelp('h', "help");
    if (!parser.Parse(argc, argv)) {
        std::cerr << "Error: wrong argument!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return EXIT_FAILURE;
    }
    if (!(parser.GetFlag("from-spb-to-pskov") || parser.GetFlag("from-pskov-to-spb"))) {
        std::cerr << "Error: please enter direction!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return EXIT_FAILURE;
    }

    if (parser.GetFlag("from-spb-to-pskov") && parser.GetFlag("from-pskov-to-spb")) {
        std::cerr << "Error: You can only choose one direction" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return EXIT_FAILURE;
    }

    if (parser.GetStringValue("date") == "") {
        std::cerr << "Error: please enter a date!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return EXIT_FAILURE;
    } else if (parser.GetStringValue("date").size() != 10 &&
               parser.GetStringValue("date")[4] != ':') {
        std::cerr << "Error: wrong fromat!" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return EXIT_FAILURE;
    }

    if (parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return EXIT_SUCCESS;
    }

    Info info;
    info.date = parser.GetStringValue("date");
    info.from = (parser.GetFlag("from-spb-to-pskov") ? "c2" : "c25");
    info.to = (parser.GetFlag("from-spb-to-pskov") ? "c25" : "c2");
    info.api_key = std::getenv("YANDEX_RASP_API_KEY");
    info.cash_file_name = info.date + ".txt";
    
    
    json response;
    const fs::path cash("cash");
    if (!fs::exists(cash/info.cash_file_name)) {
        fs::create_directory(cash);
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
        fout.open(cash/info.cash_file_name);
        fout << response.dump(4);
        fout.close();
    } else {
        std::ifstream fin;
        fin.open(cash/info.cash_file_name);
        response = json::parse(fin);
    }

    if (response.find("error") != response.end()) {
        std::cout << "Error";
    }

    size_t cnt = 1;
    for (json::iterator it = response["segments"].begin(); it != response["segments"].end(); ++it, ++cnt) {
        json cur = *it;
        
        std::cout << "Path " << cnt << '\n' << "From" << ' ' << response["search"]["from"]["popular_title"]
                  << ' ' << "to" << ' ' << response["search"]["to"]["popular_title"] << " ";
        if (cur.find("details") != cur.end()) {
            //replace with do...while
            std::cout << "with transfer via ";
            json::iterator temp = cur["details"].begin();
            std::cout << (*temp)["to"]["popular_title"];
            ++temp;
            for (int i = 1; i < kTranseferCnt; ++i) {
                std::cout << ", " << (*temp)["to"]["popular_title"];
                ++temp;
            }
        }
        std::cout << std::endl;
        if (cur.find("details") != cur.end()) {
            json::iterator temp = cur["details"].begin();
            for (int i = 1; i <= 2*kTranseferCnt+1; ++i) {
                if (i%2 == 0) {
                    std::cout << "--- "<< SecondsToTime((*temp)["duration"]) << " ---" << std::endl;
                } else {
                    std::cout << "Departure:\t" << TimestampToDate(Iso8601ToTimestamp((*temp)["departure"])) << std::endl;
                    std::cout << "Arrival:\t" << TimestampToDate(Iso8601ToTimestamp((*temp)["arrival"])) << std::endl;
                }
                ++temp;
            }
        } else {
            std::cout << "Departure:\t" << TimestampToDate(Iso8601ToTimestamp(cur["departure"])) << std::endl;
            std::cout << "Arrival:\t" << TimestampToDate(Iso8601ToTimestamp(cur["arrival"])) << std::endl;
        }
        if (cur.find("duration") != cur.end()) {
            std::cout << "Total time:\t" << SecondsToTime(cur["duration"]) << std::endl;
        } else {
            std::cout << "Total time:\t" << SecondsToTime(Iso8601ToTimestamp(cur["arrival"]) - Iso8601ToTimestamp(cur["departure"])) << std::endl;
        }
        std::cout << "Ticket price:\t" << GetTicketsPrice(cur["tickets_info"]["places"]) << std::endl;
        std::cout << std::endl;
    }
}