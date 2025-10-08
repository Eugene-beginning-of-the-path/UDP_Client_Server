cmake_minimum_required(VERSION 3.22)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project(Custom_UDP)

find_package(Boost COMPONENTS headers system REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(UDP_Client)
add_subdirectory(UDP_Server)