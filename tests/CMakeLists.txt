cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_executable(
        essentials_proposal_tests
        main.cpp
        scope_guard_test.cpp
        core_test.cpp
)

target_link_libraries(
        essentials_proposal_tests
        PUBLIC
        ${GTEST_LIBRARIES}
)