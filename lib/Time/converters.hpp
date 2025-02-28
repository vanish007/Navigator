#ifndef _CONVERTERS_
#define _CONVERTERS_

#include <iomanip>
#include <sstream>
#include <string>
#include <chrono>
#include <format>

std::string TimestampToDate(const long int seconds);

std::time_t Iso8601ToTimestamp(const std::string& iso8601);

// @bug the number of hours will exceed 24 it will not be cut
std::string SecondsToTime(size_t seconds);


#endif