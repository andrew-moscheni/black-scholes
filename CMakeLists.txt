cmake_minimum_required(VERSION 3.10.0)
project(black-scholes VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(pybind11)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
add_library(pricing_engine STATIC server/service_layer.cpp)
target_include_directories(pricing_engine PUBLIC include)
target_link_libraries(pricing_engine PRIVATE pybind11::pybind11 Python3::Module)

pybind11_add_module(pricing_engine_py server/service_layer.cpp)
target_link_libraries(pricing_engine_py
    PRIVATE
        pricing_engine
        nlohmann_json::nlohmann_json
        Python3::Module
)

target_sources(pricing_engine PRIVATE include/black-scholes.h include/option-method.h include/payoff.h)

include(CTest)
enable_testing()

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests tests/test_pricing.cpp)
target_link_libraries(run_tests PRIVATE pricing_engine gtest_main)
add_test(NAME PricingTests COMMAND run_tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)