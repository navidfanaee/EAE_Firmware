add_executable(tests_basic tests_basic.cpp)
target_link_libraries(tests_basic PRIVATE gtest_main)
add_test(NAME basic_test COMMAND tests_basic)

# --- CAN Parser test ---
add_executable(test_canparser test_canparser.cpp
    ../src/drivers/CANParser.cpp

)


target_include_directories(test_canparser PRIVATE ../include)
target_link_libraries(test_canparser PRIVATE gtest_main)
add_test(NAME canparser_test COMMAND test_canparser)

# --- State Machine test ---
add_executable(test_statemachine test_statemachine.cpp
    ../src/app/StateMachine.cpp
)
target_include_directories(test_statemachine PRIVATE ../include)
target_link_libraries(test_statemachine PRIVATE gtest_main)
add_test(NAME statemachine_test COMMAND test_statemachine)

# --- PID unit tests ---
add_executable(test_pid test_pid.cpp
    ../src/app/PID.cpp
)
target_include_directories(test_pid PRIVATE ../include)
target_link_libraries(test_pid PRIVATE gtest_main)
add_test(NAME pid_test COMMAND test_pid)


# --- Cooling Controller unit tests ---
add_executable(test_controller test_controller.cpp
    ../src/app/CoolingController.cpp
    ../src/app/StateMachine.cpp
    ../src/app/PID.cpp
)
target_include_directories(test_controller PRIVATE ../include)
target_link_libraries(test_controller PRIVATE gtest_main)
add_test(NAME controller_test COMMAND test_controller)
