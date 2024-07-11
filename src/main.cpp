#include <fstream>
#include <iostream>

#include "include/interpreter.hpp"
#include "include/cli.hpp"

constexpr char const* version = "0.0.1";

int main(int argc, char* argv[]) {
	CLI cli({ argv + 1, argv + argc });
	try {
		cli.checkout();
	} catch (const std::exception& error) {
		std::cout << error.what() << std::endl;
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
		initfile << "def main() {\n    println(\"Hello Pimpki!\") \n}\n";
		std::cout << "Successfully created a new main.zk file." << std::endl;
		return 0;
	}
	ZynkInterpreter interpreter;
	try {
		const std::vector<Token> tokens = interpreter.interpret_file(cli.args.file_path);
	} catch (const std::runtime_error& error) {
		std::cerr << error.what() << std::endl;
		return -1;
	}
}
