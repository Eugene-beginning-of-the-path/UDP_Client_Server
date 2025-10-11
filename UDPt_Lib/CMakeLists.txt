cmake_minimum_required(VERSION 3.22)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project(UDPt_Lib)

include(FetchContent)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git GIT_TAG 11.0.2)
FetchContent_MakeAvailable(fmt)

find_package(Boost COMPONENTS headers system REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(${PROJECT_NAME} ${CORE_SOURCES})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Boost::headers
        Boost::system
        OpenSSL::Crypto
        fmt::fmt)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})