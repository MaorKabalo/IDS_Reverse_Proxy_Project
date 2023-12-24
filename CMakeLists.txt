cmake_minimum_required(VERSION 3.27)
project(IDS_Project)

add_definitions(-DPROJECT_NAME="IDS_Project")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
        ReverseProxy.cpp
        EBPF_Runner.cpp)

set(HEADER_FILES ReverseProxy.h
                 EBPF_Runner.h)

set(PYTHON_FILES server.py
        client.py)

set(EBPF_FILES
        cmake-build-debug/bandwidth_limiting.bpf.c)

add_executable(IDS_Project main.cpp
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${EBPF_FILES})