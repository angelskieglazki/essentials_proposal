cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(
        scope_guard_test
        scope_guard_test.cpp
)

target_link_libraries(
        scope_guard_test
        PUBLIC
        ${GTEST_LIBRARIES}
)