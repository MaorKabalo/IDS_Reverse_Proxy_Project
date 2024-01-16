cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

add_executable(untitled main.cpp
        ReverseProxy.h
        ReverseProxy.cpp
        SockLimiting.h
        SockLimiting.cpp
        checkIP.h
        checkIP.cpp
        json.hpp)

target_link_libraries(untitled ${CURL_LIBRARIES})
include_directories(${PROJECT_SOURCE_DIR})