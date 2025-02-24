cmake_minimum_required(VERSION 3.30)

project(spb-pskov)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS OFF)

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
        
FetchContent_MakeAvailable(
    web_request
    json
)

add_subdirectory(bin)
add_subdirectory(lib)
