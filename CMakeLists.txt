CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

project(UMDF_B3_SBE VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(src)

option(PACKAGE_TESTS "Build the tests" ON)

if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

option(PACKAGE_BENCHMARK "Build benchmark" ON)

if (PACKAGE_BENCHMARK)
    add_subdirectory(benchmark)
endif()