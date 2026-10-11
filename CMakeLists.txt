cmake_minimum_required(VERSION 3.20)  # CMake version check
project(tcp-server)                   # Create project
#set(CMAKE_CXX_STANDARD 20)           # Enable c++20 standard

add_executable(
    tcp-server
    main.cpp
)

target_compile_features(tcp-server PRIVATE cxx_std_20)

target_compile_options(tcp-server PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>)
