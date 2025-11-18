cmake_minimum_required(VERSION 3.10)

project(HW4_VAR16
    VERSION 1.0.0
    DESCRIPTION "Лабораторная работа №3: Основы метапрограммирования"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(HW4_VAR16 main.cpp)
target_include_directories(HW4_VAR16 PRIVATE ${INCLUDE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(HW4_VAR16 PRIVATE -Wall -Wextra -Wpedantic)
endif()


# GoogleTest

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(INSTALL_GTEST OFF CACHE BOOL "Disable installation of googletest" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(gtests tests.cpp)
target_include_directories(gtests PRIVATE ${INCLUDE_DIR})

target_link_libraries(gtests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(gtests)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(gtests PRIVATE -Wall -Wextra -Wpedantic)
endif()