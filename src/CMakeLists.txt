cmake_minimum_required(VERSION 3.8)

set(Sources
	main.cpp
	lexer.cpp
	cli.cpp
	cli.h
	interpreter.h
	interpreter.cpp
)
add_library(ZynkLib cli.h cli.cpp)
add_executable(Zynk ${Sources})
# add_library(Zynk STATIC ${Sources})