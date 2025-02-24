#include "converters.hpp"

std::string TimestampToDate(const long int seconds) {
    std::string ans = "";
 
    const int days_of_month[] = { 31, 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31 };
 
    long int curr_year = 1970;
    long int days_till_now = seconds / (24 * 60 * 60);
    long int extra_time = seconds % (24 * 60 * 60);
    long int extra_days;
    long int index;
    long int date;
    long int month;
    long int hours; 
    long int minutes;
    long int secondss;
    bool flag = false;
 
    while (true) {
        if (curr_year % 400 == 0
            || (curr_year % 4 == 0 && curr_year % 100 != 0)) {
            if (days_till_now < 366) {
                break;
            }
            days_till_now -= 366;
        }
        else {
            if (days_till_now < 365) {
                break;
            }
            days_till_now -= 365;
        }
        curr_year += 1;
    }
    extra_days = days_till_now + 1;
 
    if (curr_year % 400 == 0
        || (curr_year % 4 == 0 && curr_year % 100 != 0)) {
        flag = true;
        }
 
    month = 0, index = 0;
    if (flag == true) {
        while (true) {
            if (index == 1) {
                if (extra_days - 29 < 0)
                    break;
                month += 1;
                extra_days -= 29;
            }
            else {
                if (extra_days - days_of_month[index] < 0) {
                    break;
                }
                month += 1;
                extra_days -= days_of_month[index];
            }
            index += 1;
        }
    }
    else {
        while (true) {
 
            if (extra_days - days_of_month[index] < 0) {
                break;
            }
            month += 1;
            extra_days -= days_of_month[index];
            index += 1;
        }
    }
 
    if (extra_days > 0) {
        month += 1;
        date = extra_days;
    }
    else {
        if (month == 2 && flag == 1)
            date = 29;
        else {
            date = days_of_month[month - 1];
        }
    }
 
    hours = extra_time / 3600;
    minutes = (extra_time % 3600) / 60;
    secondss = (extra_time % 3600) % 60;
 
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
    ans += std::to_string(curr_year);
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

// @bug the number of hours will exceed 24 it will not be cut
std::string SecondsToTime(size_t seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secondss = (seconds % 3600) % 60;

    return std::format("{} hours and {} minutes", hours, minutes);
}
