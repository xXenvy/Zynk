cmake_minimum_required(VERSION 3.8)

set(Sources
	lexer.cpp
	cli.cpp
	interpreter.cpp
	parser.cpp
	runtime.cpp
	evaluator.cpp
	gc.cpp
)
set(Headers
	include/cli.hpp
	include/interpreter.hpp
	include/lexer.hpp
	include/parser.hpp
	include/ast.hpp
	include/errors.hpp
	include/runtime.hpp
	include/evaluator.hpp
	include/gc.hpp
	include/block.hpp
)
add_library(ZynkLib ${Sources} ${Headers})
add_executable(Zynk main.cpp ${Sources} ${Headers})