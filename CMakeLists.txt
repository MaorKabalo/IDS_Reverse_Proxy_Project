cmake_minimum_required(VERSION 3.27)
project(IDS_Project)

# Define project name
add_definitions(-DPROJECT_NAME="IDS_Project")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Define directories
set(PCAPPLUSPLUS_SOURCE_DIR "/home/kali/Desktop/PcapPlusPlus")
set(PCAPPLUSPLUS_BINARY_DIR "/home/kali/Desktop/PcapPlusPlus/build")

# Include directories
include_directories(
        ${PCAPPLUSPLUS_SOURCE_DIR}/Common++/header
        ${PCAPPLUSPLUS_SOURCE_DIR}/Pcap++/header
        ${PCAPPLUSPLUS_SOURCE_DIR}/Packet++/header
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/Headers
)

# Define source files
file(GLOB SOURCE_FILES
        Source/*.cpp
)

# Define header files (not needed for compilation, but useful for IDEs)
file(GLOB HEADER_FILES
        Headers/*.h
        Headers/IPTools/*.h
)

# Define Python and eBPF files (these are not used in the build but may be useful for other purposes)
set(PYTHON_FILES
        Python/server.py
        Python/rate_limiting_tester.py
)

set(EBPF_FILES
        cmake-build-debug/bandwidth_limiting.bpf.c
        cmake-build-debug/rate_limiting.bpf.c
)

# Add PcapPlusPlus
add_subdirectory(${PCAPPLUSPLUS_SOURCE_DIR} ${PCAPPLUSPLUS_BINARY_DIR})

# Create executable
add_executable(IDS_Project ${SOURCE_FILES})

# Link libraries
target_link_libraries(IDS_Project
        Pcap++
        ${CURL_LIBRARIES}
        ${LIBBPF_LIBRARIES}
)

# Find and include CURL
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(IDS_Project ${CURL_LIBRARIES})

# Find and include Libbpf
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBBPF REQUIRED libbpf)
target_include_directories(IDS_Project PRIVATE ${LIBBPF_INCLUDE_DIRS})
link_directories(${LIBBPF_LIBRARY_DIRS})

