cmake_minimum_required(VERSION 3.8)
# include_directories(boost_1_85_0)

set(Sources
	lexer.cpp
	cli.cpp
	interpreter.cpp
)
set(Headers
	include/cli.hpp
	include/interpreter.hpp
	include/lexer.hpp
)
add_library(ZynkLib ${Sources} ${Headers})
add_executable(Zynk main.cpp ${Sources} ${Headers})