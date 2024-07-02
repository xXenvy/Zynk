cmake_minimum_required(VERSION 3.8)

set(Sources 
	test_cli.cpp
)

include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main)
FetchContent_MakeAvailable(googletest)

add_executable(Tests ${Sources})
target_link_libraries(Tests PUBLIC gtest_main ZynkLib)
add_test(
    NAME Tests
    COMMAND Tests
)