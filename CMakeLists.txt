cmake_minimum_required(VERSION 3.5)
project(DSS_IF)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)

# fetch latest argparse
include(FetchContent)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)



add_executable(dss_if main.cpp)
target_link_libraries(dss_if PRIVATE argparse  DssApi)

add_executable(fromJson jsonToDss.cpp)
target_link_libraries(fromJson PRIVATE argparse  DssApi)

add_executable(toJson DssToJson.cpp)
target_link_libraries(toJson PRIVATE argparse  DssApi)