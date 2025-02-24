#include "json.hpp"

std::string GetTicketsPrice(json r) {
    for (json::iterator it = r.begin(); it != r.end(); ++it) {
        json cur = *it;
        return cur["price"]["whole"].dump(0) + " RUB";
    }
    return "-";
}

void PrintPath(json cur, json response, const ApiInfo info, size_t& cnt) {
    std::cout << "\t\t### Path " << cnt << " ###" << '\n';
    std::cout << "From" << ' ' << response["search"]["from"]["popular_title"]
                << ' ' << "to" << ' ' << response["search"]["to"]["popular_title"] << " ";
    if (cur.find("details") != cur.end()) {
        std::cout << "with transfer via ";
        json::iterator temp = cur["details"].begin();
        int i = 1;
        do {
            std::cout << (*temp)["to"]["popular_title"];
            ++temp;
            ++i;
        } while (i < info.max_transfer_cnt);
    }
    std::cout << std::endl;
    if (cur.find("details") != cur.end()) {
        json::iterator temp = cur["details"].begin();
        for (int i = 1; i <= 2*info.max_transfer_cnt+1; ++i) {
            if (i%2 == 0) {
                std::cout << "--- "<< SecondsToTime((*temp)["duration"]) << " ---" << std::endl;
            } else {
                std::cout << "Transport type:\t" << (*temp)["thread"]["transport_type"] << std::endl;
                std::cout << "Departure:\t" << TimestampToDate(Iso8601ToTimestamp((*temp)["departure"])) << '\t' << "("<<(*temp)["departure_from"]["popular_title"]<<")" << std::endl;
                std::cout << "Arrival:\t" << TimestampToDate(Iso8601ToTimestamp((*temp)["arrival"]))<< '\t' << "("<<cur["arrrival_to"]["popular_title"]<<")" << std::endl;
            }
            ++temp;
        }
    } else {
        std::cout << "Transport type:\t" << cur["from"]["transport_type"] << std::endl;
        std::cout << "Departure:\t" << TimestampToDate(Iso8601ToTimestamp(cur["departure"])) << '\t' << "("<<cur["from"]["popular_title"]<<")"  << std::endl;
        std::cout << "Arrival:\t" << TimestampToDate(Iso8601ToTimestamp(cur["arrival"])) << '\t' << "("<<cur["to"]["popular_title"]<<")"<< std::endl;
    }
    if (cur.find("duration") != cur.end()) {
        std::cout << "Total time:\t" << SecondsToTime(cur["duration"]) << std::endl;
    } else {
        std::cout << "Total time:\t" << SecondsToTime(Iso8601ToTimestamp(cur["arrival"]) - Iso8601ToTimestamp(cur["departure"])) << std::endl;
    }
    std::cout << "Ticket price:\t" << GetTicketsPrice(cur["tickets_info"]["places"]) << std::endl;
    std::cout << std::endl;
}