cmake_minimum_required(VERSION 3.0.0)
project(tests VERSION 0.1.0)


include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
find_package(GTest REQUIRED)

message(${CMAKE_CXX_COMPILER})
set(CMAKE_CXX_STANDARD 20)
MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
include(cmake/warn-everything.cmake)

warn_everything(WARN_EVERYTHING_CXX WARN_EVERYTHING_C)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARN_EVERYTHING_C}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_EVERYTHING_CXX}")

set(COMMON_WARN_FLAGS "-pipe -fno-common -Wall -Wextra -Wno-shadow -Winit-self -Wcast-align -Wformat=2 -Wwrite-strings -Wstrict-overflow=5 -pedantic -Wcast-qual -Wconversion")
set(COMMON_WARN_FLAGS "${COMMON_WARN_FLAGS}  -Wdisabled-optimization -Wmissing-include-dirs -Wswitch-default -Wundef -pedantic-errors -Wsizeof-array-div -Wmisleading-indentation")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_WARN_FLAGS}")

set(CXX_WARN_FLAGS "-Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wnarrowing -Wnoexcept-type -Wregister -Wno-old-style-cast -Woverloaded-virtual -Wsign-promo")# -Weffc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARN_FLAGS}")


add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(benchmarks)
