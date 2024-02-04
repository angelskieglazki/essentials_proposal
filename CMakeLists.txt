cmake_minimum_required(VERSION 3.0.0)
project(tests VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(benchmarks)
