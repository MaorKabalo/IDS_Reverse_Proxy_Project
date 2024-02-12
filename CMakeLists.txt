cmake_minimum_required(VERSION 3.27)
project(IDS_Project)

add_definitions(-DPROJECT_NAME="IDS_Project")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp
        ReverseProxy.cpp
        SockLimiting.cpp
        EBPF_Runner.cpp
        Port_Scanning_Detector.cpp)

set(HEADER_FILES ReverseProxy.h
        SockLimiting.h
        EBPF_Runner.h
        Port_Scanning_Detector.h)

set(PYTHON_FILES server.py
        rate_limiting_tester.py)

set(EBPF_FILES cmake-build-debug/bandwidth_limiting.bpf.c
        cmake-build-debug/rate_limiting.bpf.c)

set(PCAPPLUSPLUS_SOURCE_DIR "/home/kali/Desktop/PcapPlusPlus")
set(PCAPPLUSPLUS_BINARY_DIR "/home/kali/Desktop/PcapPlusPlus/build")

# Add PcapPlusPlus source code to the project
add_subdirectory(${PCAPPLUSPLUS_SOURCE_DIR} ${PCAPPLUSPLUS_BINARY_DIR})

# Add PcapPlusPlus include directories to the project
include_directories(${PCAPPLUSPLUS_SOURCE_DIR}/Common++/header)
include_directories(${PCAPPLUSPLUS_SOURCE_DIR}/Pcap++/header)
include_directories(${PCAPPLUSPLUS_SOURCE_DIR}/Packet++/header)

# Link against the library targets
add_executable(IDS_Project
        ${SOURCE_FILES}
        ${HEADER_FILES}
        ${PYTHON_FILES})

target_link_libraries(IDS_Project Pcap++)

# Add the source directory to include directories
include_directories(${PROJECT_SOURCE_DIR})
