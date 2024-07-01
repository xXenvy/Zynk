#include <iostream>
#include "cli.h"

constexpr char const* version = "0.0.1";

int main(int argc, char* argv[]) {
	CLI cli(std::vector<std::string>(argv + 1, argv + argc));
	try {
		cli.checkout();
	} catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
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
}
