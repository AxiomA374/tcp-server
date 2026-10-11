cmake_minimum_required(VERSION 3.20)  # CMake version check
project(tcp-server)                   # Create project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_STANDARD 20)           # Enable c++20 standard

add_executable(
    tcp-server
    main.cpp
    src/Server.cpp
)

target_compile_features(tcp-server PRIVATE cxx_std_20)

target_compile_options(tcp-server PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>)

target_include_directories(tcp-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
