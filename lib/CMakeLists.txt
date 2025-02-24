add_library(Parse 
            Parse/parser.cpp 
            Parse/parser.hpp)
target_include_directories(Parse PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Time 
            Time/converters.cpp 
            Time/converters.hpp)
target_include_directories(Time PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ArgParser 
            ArgParser/arg_parser.cpp 
            ArgParser/arg_parser.hpp)
target_include_directories(ArgParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Number 
            Number/number.cpp 
            Number/number.hpp)
target_include_directories(Number PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Print
            Print/json.cpp 
            Print/json.hpp)
target_include_directories(Print PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Print PUBLIC 
                      nlohmann_json::nlohmann_json 
                      ftxui::screen
                      ftxui::dom
                      ftxui::component)
