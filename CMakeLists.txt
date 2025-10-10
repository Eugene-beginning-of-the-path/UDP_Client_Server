cmake_minimum_required(VERSION 3.22)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project(Custom_UDP)

include(FetchContent)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.0.2)
FetchContent_MakeAvailable(fmt)

find_package(Boost COMPONENTS headers system REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(UDP_Client)
add_subdirectory(UDP_Server)