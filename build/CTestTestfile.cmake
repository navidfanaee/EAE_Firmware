# CMake generated Testfile for 
# Source directory: /home/vboxuser/EAE_Firmware
# Build directory: /home/vboxuser/EAE_Firmware/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(statemachine_test "/home/vboxuser/EAE_Firmware/build/test_statemachine")
set_tests_properties(statemachine_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;38;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
add_test(coolingcontroller_test "/home/vboxuser/EAE_Firmware/build/test_coolingcontroller")
set_tests_properties(coolingcontroller_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;42;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
