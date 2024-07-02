cmake_minimum_required(VERSION 3.8)
add_subdirectory(googletest)

add_executable(Tests test_main.cpp)
target_link_libraries(Tests PUBLIC gtest_main ZynkLib)
add_test(
	NAME Tests
	COMMAND Tests
)