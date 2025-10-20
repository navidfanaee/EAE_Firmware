cmake_minimum_required(VERSION 3.14)
project(EAE_Firmware LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# ----- Libraries -----
add_library(drivers STATIC
    src/drivers/CANParser.cpp
)
target_include_directories(drivers PUBLIC include)

add_library(appcore STATIC
    src/app/StateMachine.cpp
    src/app/PID.cpp
    src/app/CoolingController.cpp
)
target_include_directories(appcore PUBLIC include)
target_link_libraries(appcore PUBLIC drivers)

# ----- Firmware runtime -----
add_executable(eae_firmware src/main.cpp)
target_include_directories(eae_firmware PRIVATE include)
target_link_libraries(eae_firmware PRIVATE appcore drivers)

# ----- Tests -----
add_executable(test_statemachine tests/test_statemachine.cpp)
target_link_libraries(test_statemachine PRIVATE appcore gtest_main)
add_test(NAME statemachine_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_statemachine)
# ----- CoolingController Tests -----
add_executable(test_controller tests/test_controller.cpp)
target_link_libraries(test_controller PRIVATE appcore drivers gtest_main)
add_test(NAME coolingcontroller_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_coolingcontroller)
