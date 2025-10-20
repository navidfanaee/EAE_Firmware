# CMake generated Testfile for 
# Source directory: /home/vboxuser/EAE_Firmware
# Build directory: /home/vboxuser/EAE_Firmware/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(canparser_test "/home/vboxuser/EAE_Firmware/build/test_canparser")
set_tests_properties(canparser_test PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/EAE_Firmware/CMakeLists.txt;27;add_test;/home/vboxuser/EAE_Firmware/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
