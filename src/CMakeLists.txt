set(Sources
	parsing/lexer.cpp
	parsing/parser.cpp
	execution/evaluator.cpp
	execution/runtime.cpp
	execution/interpreter.cpp
	execution/typechecker/checker.cpp
	execution/gc/gc.cpp
	execution/gc/object.cpp
	cli/cli.cpp
)
set(Headers
	parsing/include/lexer.hpp
	parsing/include/parser.hpp
	parsing/include/ast.hpp
	parsing/include/token.hpp

	execution/include/evaluator.hpp
	execution/include/runtime.hpp
	execution/include/interpreter.hpp
	execution/gc/include/gc.hpp
	execution/gc/include/block.hpp
	execution/gc/include/object.hpp

	execution/typechecker/include/checker.hpp
	cli/include/cli.hpp
	errors/include/errors.hpp
)
add_library(ZynkLib ${Sources} ${Headers})
add_executable(Zynk main.cpp ${Sources} ${Headers})