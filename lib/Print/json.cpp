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
            print_info.name.append((*temp)["to"]["popular_title"]);
            ++temp;
            ++i;
        } while (i < api_info.max_transfer_cnt);
    }

    if (cur.find("details") != cur.end()) {
        json::iterator temp = cur["details"].begin();
        for (int i = 1; i <= 2*api_info.max_transfer_cnt+1; ++i) {
            if (i%2 == 0) {
                print_info.transfer_duration = SecondsToTime((*temp)["duration"]);
            } else {
                print_info.transport_type = (*temp)["thread"]["transport_type"];
                print_info.departure = TimestampToDate(Iso8601ToTimestamp((*temp)["departure"]));
                print_info.departure.append("(");
                print_info.departure.append(((*temp)["departure_from"]["popular_title"].is_null() ? "" : (*temp)["departure_from"]["popular_title"]));
                print_info.departure.append(")");
                print_info.arrival = TimestampToDate(Iso8601ToTimestamp((*temp)["arrival"]));
                print_info.arrival.append("(");
                print_info.arrival.append(((*temp)["arrival_to"]["popular_title"].is_null() ? "" : (*temp)["arrival_to"]["popular_title"]));
                print_info.arrival.append(")");
            }
            ++temp;
        }
    } else {
        print_info.transport_type = cur["thread"]["transport_type"];
        print_info.departure = TimestampToDate(Iso8601ToTimestamp(cur["departure"]));
        print_info.departure.append("(");
        print_info.departure.append((cur["from"]["popular_title"].is_null() ? "" : cur["from"]["popular_title"]));
        print_info.departure.append(")");
        print_info.arrival = TimestampToDate(Iso8601ToTimestamp(cur["arrival"]));
            print_info.arrival.append("(");
            print_info.arrival.append((cur["to"]["popular_title"].is_null() ? "" : cur["to"]["popular_title"]));
            print_info.arrival.append(")");
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
    
    auto table = ftxui::Table({
        {"Path", print_info.path},
        {"Name", print_info.name},
        {"Transport type", print_info.transport_type},
        {"Departure", print_info.departure},
        {"Arrival", print_info.arrival},
        {"Total time", print_info.total_time},
        {"Ticket price", print_info.ticket_price},
    });
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