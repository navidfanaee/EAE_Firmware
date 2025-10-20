# CMake generated Testfile for 
# Source directory: /home/vboxuser/EAE_Firmware
# Build directory: /home/vboxuser/EAE_Firmware/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(statemachine_test "/home/vboxuser/EAE_Firmware/build/test_statemachine")
set_tests_properties(statemachine_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;38;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
add_test(coolingcontroller_test "/home/vboxuser/EAE_Firmware/build/test_controller")
set_tests_properties(coolingcontroller_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;42;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
add_test(pid_test "/home/vboxuser/EAE_Firmware/build/test_pid")
set_tests_properties(pid_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;46;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
add_test(canparser_test "/home/vboxuser/EAE_Firmware/build/test_canparser")
set_tests_properties(canparser_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;50;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
