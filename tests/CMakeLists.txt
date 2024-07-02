cmake_minimum_required(VERSION 3.8)

include(FetchContent)
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main)
FetchContent_MakeAvailable(googletest)

add_executable(Tests test_main.cpp)
target_link_libraries(Tests PUBLIC gtest_main ZynkLib)
add_test(
	NAME Tests
	COMMAND Tests
)