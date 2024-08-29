cmake_minimum_required(VERSION 3.27)
project(IDS_Project)

add_definitions(-DPROJECT_NAME="IDS_Project")

#add_definitions(-DPROJECT_NAME="untitled")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES Source/main.cpp
        Source/ReverseProxy.cpp
        Source/SockLimiting.cpp
        Source/EBPF_Runner.cpp
        Source/Port_Scanning_Detector.cpp
        Source/checkIP.cpp
        Source/BlockIP.cpp
        Source/IPTools.cpp)

set(HEADER_FILES Headers/ReverseProxy.h
        Headers/SockLimiting.h
        Headers/EBPF_Runner.h
        Headers/Port_Scanning_Detector.h
        Headers/IPTools/checkIP.h
        Headers/IPTools/BlockIP.h
        Headers/IPTools/IPTools.h
)

set(PYTHON_FILES Python/server.py
        Python/rate_limiting_tester.py)

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
include_directories(${PROJECT_SOURCE_DIR}/Headers)

#Include CURL
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(IDS_Project ${CURL_LIBRARIES})

#Include Libbpf
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBBPF REQUIRED libbpf)
include_directories(${LIBBPF_INCLUDE_DIRS})
link_directories(${LIBBPF_LIBRARY_DIRS})
target_link_libraries(IDS_Project ${LIBBPF_LIBRARIES})