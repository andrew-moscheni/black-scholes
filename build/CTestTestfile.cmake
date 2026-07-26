# CMake generated Testfile for 
# Source directory: /home/agmoscheni/black-scholes
# Build directory: /home/agmoscheni/black-scholes/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(PricingTests "/home/agmoscheni/black-scholes/build/run_tests")
set_tests_properties(PricingTests PROPERTIES  _BACKTRACE_TRIPLES "/home/agmoscheni/black-scholes/CMakeLists.txt;46;add_test;/home/agmoscheni/black-scholes/CMakeLists.txt;0;")
subdirs("_deps/json-build")
subdirs("_deps/pybind11-build")
subdirs("_deps/googletest-build")
