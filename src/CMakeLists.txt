cmake_minimum_required(VERSION 3.8)

set(Sources
	parsing/lexer.cpp
	parsing/parser.cpp
	execution/evaluator.cpp
	execution/runtime.cpp
	gc/gc.cpp
	cli.cpp
	interpreter.cpp
)
set(Headers
	parsing/include/lexer.hpp
	parsing/include/parser.hpp
	parsing/include/ast.hpp
	parsing/include/token.hpp

	execution/include/evaluator.hpp
	execution/include/runtime.hpp
	gc/include/gc.hpp
	gc/include/block.hpp

	include/cli.hpp
	include/interpreter.hpp
	include/errors.hpp
)
add_library(ZynkLib ${Sources} ${Headers})
add_executable(Zynk main.cpp ${Sources} ${Headers})