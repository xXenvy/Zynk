#include <fstream>
#include <iostream>

#include "include/interpreter.hpp"
#include "include/cli.hpp"
#include "include/errors.hpp"

constexpr char const* version = "0.0.1";

int main(int argc, char* argv[]) {
	CLI cli({ argv + 1, argv + argc });
	try {
		cli.checkout();
	} catch (const ZynkError& error) {
		error.print();
		return -1;
	}
	if (cli.args.help) {
		cli.show_help();
		return 0;
	}
	if (cli.args.version) {
		std::cout << "Version: " << version << std::endl;
		return 0;
	}
	if (cli.args.init) {
		std::ofstream initfile("main.zk");
		initfile << "def main() {\n    println(\"Hello Pimpki!\"); \n}\nmain();";
		std::cout << "Successfully created a new main.zk file." << std::endl;
		return 0;
	}

	ZynkInterpreter interpreter;
	try {
		const ASTProgram* program = interpreter.interpret_file(cli.args.file_path);
		delete program;
	} catch (const ZynkError& error) {
		error.print();
		return -1;
	} catch (const std::exception& unknown_error) {
		// Unknown error type, constructing a PanicError.
		ZynkError{
			ZynkErrorType::PanicError,
			std::string("The interpreter unexpectedly panicked. Additional info: \"") + unknown_error.what() + "\".\n"
		}.print();
		return -1;
	}
}
