#include <iostream>
#include "cli.h"

bool Arguments::empty() const {
	return file_path.empty() && !help && !version && !init;
}

CLI::CLI(const std::vector<std::string>& raw_args) {
	for (const std::string& arg : raw_args) {
		if (arg.find(".zk", 0) != -1) args.file_path = arg;
		else if (arg.find("help", 0) != -1) args.help = true;
		else if (arg.find("version", 0) != -1) args.version = true;
		else if (arg.find("init", 0) != -1) args.init = true;
	}
}

void CLI::checkout() const {
	if (args.empty()) {
		throw std::logic_error("No argument was given. Consider using --help.");
	}
	if (!args.file_path.empty() && args.help) {
		throw std::logic_error("Too many arguments.");
	}
}

void CLI::show_help() const {
	std::cout << std::endl << "<----- Zynk Help ----->" << std::endl;
	std::cout << "Zynk - A simple interpreted programming language, written in C++." << std::endl << std::endl;
	std::cout << "Example Usage:\n >> Zynk main.zk\n\n";
	std::cout << "Arguments:\n"
		" --file_path <path>: Specifies the path to the script file that you want to interpret.\n"
		" --init: Initializes a basic script file template in the current directory.\n"
		" --version: Displays the current version of Zynk interpreter.\n"
		" --help: Displays this help message.\n";
}