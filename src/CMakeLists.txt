cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(base64
        GIT_REPOSITORY https://github.com/matheusgomes28/base64pp
        GIT_TAG main)

FetchContent_MakeAvailable(base64)

add_library(DssApi  STATIC
            DssCmn.h
            SeatStatus.h   SeatStatus.cpp
            ServerStatus.h ServerStatus.cpp
            DssStatus.h    DssStatus.cpp
            JsonUtils.h    JsonUtils.cpp)

target_link_libraries(DssApi PUBLIC nlohmann_json base64pp)
target_include_directories(DssApi PUBLIC ${CMAKE_CURRENT_LIST_DIR})