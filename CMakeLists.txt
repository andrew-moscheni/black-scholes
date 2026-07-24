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
add_library(pricing_engine STATIC
    main.cpp
)
target_include_directories(pricing_engine PUBLIC include)

pybind11_add_module(pricing_engine_py service_layer.cpp)
target_link_libraries(pricing_engine_py
    PRIVATE
        pricing_engine
        nlohmann_json::nlohmann_json
)

add_executable(black-scholes main.cpp service_layer.cpp)
target_sources(black-scholes PRIVATE black-scholes.h option-method.h payoff.h)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)