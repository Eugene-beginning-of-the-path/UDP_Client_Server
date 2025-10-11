cmake_minimum_required(VERSION 3.22)

project(UDP_Server LANGUAGES CXX)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        UDPt_Lib)