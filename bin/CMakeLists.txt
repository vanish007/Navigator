add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ArgParser 
    Number
    cpr::cpr
    nlohmann_json::nlohmann_json
    Parse
    Time
    Print
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
)
