cmake_minimum_required(VERSION 3.15)
project(EAE_Firmware LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Library under test
add_library(drivers STATIC
    src/drivers/CANParser.cpp
)
target_include_directories(drivers PUBLIC include)

# Unit test
add_executable(test_canparser tests/test_canparser.cpp)
target_link_libraries(test_canparser PRIVATE drivers gtest_main)
add_test(NAME canparser_test COMMAND test_canparser)
