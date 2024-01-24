cmake_minimum_required(VERSION 3.27)
project(IDS_Project)

add_definitions(-DPROJECT_NAME="IDS_Project")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
        ReverseProxy.cpp
        SockLimiting.cpp)

set(HEADER_FILES ReverseProxy.h
        SockLimiting.h
        Session.h)

set(PYTHON_FILES server.py)

add_executable(IDS_Project main.cpp
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${PYTHON_FILES})