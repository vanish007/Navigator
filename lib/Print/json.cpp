#include "json.hpp"

std::string GetTicketsPrice(json r) {
    for (json::iterator it = r.begin(); it != r.end(); ++it) {
        json cur = *it;
        return cur["price"]["whole"].dump(0) + " RUB";
    }
    return "-";
}

PrintInfo GetPrintPathInfo(json cur, json response, const ApiInfo api_info, size_t& cnt) {
    PrintInfo print_info;
    print_info.path = std::to_string(cnt);
    print_info.name = "From ";
    print_info.name.append(response["search"]["from"]["popular_title"]);
    print_info.name.append(" to ");
    print_info.name.append(response["search"]["to"]["popular_title"]);
    if (cur.find("details") != cur.end()) {
        print_info.name.append(" with transfer via ");
        json::iterator temp = cur["details"].begin();
        int i = 1;
        do {
            if (i > 1) {
                print_info.name.append(", ");
            }
            print_info.name.append((*temp)["to"]["popular_title"]);
            ++temp;
            ++temp;
            ++i;
        } while (i < cur["details"].size()/2+1);
    }
    
    if (cur.find("details") != cur.end()) {
        json::iterator temp = cur["details"].begin();
        int total_paths_cnt = (cur["details"].size())/2;
        if (total_paths_cnt <= api_info.max_transfer_cnt) {
            for (int i = 1; i <= cur["details"].size(); ++i) {
                if (i%2 == 0) {
                    print_info.transfer_duration.push_back(SecondsToTime((*temp)["duration"]));
                } else {
                    print_info.transport_type.push_back((*temp)["thread"]["transport_type"]);
                    std::string s = TimestampToDate(Iso8601ToTimestamp((*temp)["departure"]));
                    s.append((*temp)["from"]["title"].is_null() ? "" : " (");
                    s.append((*temp)["from"]["title"].is_null() ? "" : (*temp)["from"]["title"]);
                    s.append((*temp)["from"]["title"].is_null() ? "" : ")");
                    print_info.departure.push_back(s);
                    s = TimestampToDate(Iso8601ToTimestamp((*temp)["arrival"]));
                    s.append((*temp)["to"]["title"].is_null() ? "" : " (");
                    s.append(((*temp)["to"]["title"].is_null() ? "" : (*temp)["to"]["title"]));
                    s.append((*temp)["to"]["title"].is_null() ? "" : ")");
                    print_info.arrival.push_back(s);
                }
                ++temp;
            }
        }
    } else {
        print_info.transport_type.push_back(cur["thread"]["transport_type"]);
        std::string s = TimestampToDate(Iso8601ToTimestamp(cur["departure"]));
        s.append(cur["from"]["popular_title"].is_null() ? "" : " (");
        s.append((cur["from"]["popular_title"].is_null() ? "" : cur["from"]["popular_title"]));
        s.append(cur["from"]["popular_title"].is_null() ? "" : ")");
        print_info.departure.push_back(s);
        s =  TimestampToDate(Iso8601ToTimestamp(cur["arrival"]));
        s.append(cur["to"]["popular_title"].is_null() ? "" :" (");
        s.append((cur["to"]["popular_title"].is_null() ? "" : cur["to"]["popular_title"]));
        s.append(cur["to"]["popular_title"].is_null() ? "" :")");
        print_info.arrival.push_back(s);
    }
    if (cur.find("duration") != cur.end()) {
        print_info.total_time = SecondsToTime(cur["duration"]);
    } else {
        print_info.total_time = SecondsToTime(Iso8601ToTimestamp(cur["arrival"]) - Iso8601ToTimestamp(cur["departure"]));
    }
    print_info.ticket_price = GetTicketsPrice(cur["tickets_info"]["places"]);

    return print_info;
}

void PrintPath(json cur, json response, const ApiInfo api_info, size_t& cnt) {
    PrintInfo print_info = GetPrintPathInfo(cur, response, api_info, cnt);
    if (print_info.transfer_duration.empty() && cur.find("details") != cur.end()) {
        return;
    }

    std::vector<std::vector<std::string>> vec = {
        {"Path", print_info.path},
        {"Name", print_info.name},
    };
    for (int i = 0; i < print_info.departure.size(); ++i) {
        if (i >= 1) {
            vec.push_back({"Layover time", print_info.transfer_duration[i-1]});
        }
        vec.push_back({"Transport type", print_info.transport_type[i]});        
        vec.push_back({"Departure", print_info.departure[i]});
        vec.push_back({"Arrival", print_info.arrival[i]});
    }
    vec.push_back({"Total time", print_info.total_time});
    vec.push_back({"Ticket price", print_info.ticket_price});
    auto table = ftxui::Table(vec);
    table.SelectAll().Border(LIGHT);
    table.SelectColumn(0).Border(LIGHT);
    table.SelectRow(0).Decorate(bold);
    table.SelectRow(0).SeparatorVertical(LIGHT);
    table.SelectRow(0).Border(DOUBLE);

    auto content = table.SelectRows(1, -1);
    content.DecorateCellsAlternateRow(color(Color::Blue), 3, 0);
    content.DecorateCellsAlternateRow(color(Color::Cyan), 3, 1);
    content.DecorateCellsAlternateRow(color(Color::White), 3, 2);

    auto document = table.Render();
    auto screen =
        Screen::Create(Dimension::Fit(document));
    Render(screen, document);
    screen.Print();
    std::cout << std::endl;
}