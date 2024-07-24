cmake_minimum_required(VERSION 3.8)

set(Sources 
	test_cli.cpp
    test_lexer.cpp
    test_interpreter.cpp
    test_parser.cpp
    test_evaluator.cpp
    test_gc.cpp
    test_block.cpp
    test_runtime.cpp
)
set(GoogleTestVersion v1.14.0)

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