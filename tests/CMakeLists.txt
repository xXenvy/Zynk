cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Sources 
	test_cli.cpp
    test_lexer.cpp
    test_interpreter.cpp
    test_parser.cpp
    test_evaluator.cpp
    test_block.cpp
    test_gc.cpp
    test_runtime.cpp
    test_typechecker.cpp
)
set(GoogleTestVersion v1.15.0)

include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG ${GoogleTestVersion})
FetchContent_MakeAvailable(googletest)

add_executable(Tests ${Sources})
target_link_libraries(Tests PUBLIC gtest_main ZynkLib)
add_test(
    NAME Tests
    COMMAND Tests
)