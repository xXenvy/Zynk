set(Sources
	parsing/lexer.cpp
	parsing/parser.cpp
	execution/evaluator.cpp
	execution/runtime.cpp
	execution/interpreter.cpp
	gc/gc.cpp
	gc/object.cpp
	common/cli.cpp
	typechecker/checker.cpp
)
set(Headers
	parsing/include/lexer.hpp
	parsing/include/parser.hpp
	parsing/include/ast.hpp
	parsing/include/token.hpp

	execution/include/evaluator.hpp
	execution/include/runtime.hpp
	execution/include/interpreter.hpp
	gc/include/gc.hpp
	gc/include/block.hpp
	gc/include/object.hpp

	typechecker/include/checker.hpp
	common/include/cli.hpp
	common/include/errors.hpp
)
add_library(ZynkLib ${Sources} ${Headers})
add_executable(Zynk main.cpp ${Sources} ${Headers})