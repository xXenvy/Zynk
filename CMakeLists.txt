cmake_minimum_required (VERSION 3.8)

if(MSVC)
    add_compile_options(/W4 /WX)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Werror)
endif()

project(Zynk VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(gtest_force_shared_crt on)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)