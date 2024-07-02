cmake_minimum_required (VERSION 3.8)

project(Zynk VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(gtest_force_shared_crt on)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)