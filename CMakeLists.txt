cmake_minimum_required(VERSION 3.27)
project(untitled)

add_definitions(-DPROJECT_NAME="untitled")

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
        json.hpp
        BlockIP.cpp
        BlockIP.h
        EBPF_Runner.h
        EBPF_Runner.cpp)

target_link_libraries(untitled ${CURL_LIBRARIES})
include_directories(${PROJECT_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBBPF REQUIRED libbpf)

include_directories(${LIBBPF_INCLUDE_DIRS})
link_directories(${LIBBPF_LIBRARY_DIRS})

target_link_libraries(untitled ${LIBBPF_LIBRARIES})