cmake_minimum_required(VERSION 3.30)

project(spb-pskov)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

include(FetchContent)

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

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG        cdf28903a7781f97ba94d30b79c3a4b0c97ccce7
)
        
FetchContent_MakeAvailable(
    web_request
    json
    ftxui
)

add_subdirectory(bin)
add_subdirectory(lib)
