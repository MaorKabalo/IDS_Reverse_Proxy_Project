cmake_minimum_required(VERSION 3.27)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project main.cpp
        cmake-build-debug/hello.bpf.c
        EBPF_Runner.cpp
        EBPF_Runner.h
        cmake-build-debug/http_filter.bpf.c
)
