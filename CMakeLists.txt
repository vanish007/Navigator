cmake_minimum_required(VERSION 3.30)

project(spb-pskov)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)

include(FetchContent)

add_executable(${PROJECT_NAME} main.cpp)

FetchContent_Declare(
    Parser
    GIT_REPOSITORY https://github.com/is-itmo-c-24/labwork4-vanish007
    GIT_TAG        051a57c9d19a405303c38bc1e605f86368832346
)
    
FetchContent_Declare(
    web_request
    GIT_REPOSITORY https://github.com/libcpr/cpr
    GIT_TAG        dec9422db3af470641f8b0d90e4b451c4daebf64
)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG        9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03
)
        
FetchContent_MakeAvailable(
    Parser
    web_request
    json
)

target_include_directories(spb-pskov PRIVATE 
    ${parser_SOURCE_DIR}/lib 
)
    
    
target_link_libraries(spb-pskov PRIVATE 
    Parser
    argparser 
    number
    cpr::cpr
    nlohmann_json::nlohmann_json
)
