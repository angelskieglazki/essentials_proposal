cmake_minimum_required(VERSION 3.10)

find_package(benchmark REQUIRED)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR})

add_executable(
        test_benchmark
        test_benchmark.cpp      
)

target_link_libraries(
        test_benchmark
        PUBLIC
        ${GTEST_LIBRARIES}
        benchmark::benchmark
)