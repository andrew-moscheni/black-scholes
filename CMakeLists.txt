cmake_minimum_required(VERSION 3.10.0)
project(black-scholes VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(black-scholes main.cpp)
target_sources(black-scholes PRIVATE black-scholes.h option-method.h payoff.h)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)