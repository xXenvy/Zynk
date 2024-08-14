#include <iostream>
#include "include/cli.hpp"
#include "include/errors.hpp"

bool Arguments::empty() const {
	return file_path.empty() && !help && !version && !init;
}

CLI::CLI(const std::vector<std::string>& raw_args) : args(raw_args.size()) {
	for (const std::string& arg : raw_args) {
		if (arg.find(".zk", 0) != std::string::npos) args.file_path = arg;
		else if (arg.find("help", 0) != std::string::npos) args.help = true;
		else if (arg.find("version", 0) != std::string::npos) args.version = true;
		else if (arg.find("init", 0) != std::string::npos) args.init = true;
		else if (arg.find("--file", 0) != std::string::npos) args.count--;
	}
}

void CLI::checkout() const {
	if (args.empty()) {
		throw ZynkError(
			ZynkErrorType::CLIError,
			"No argument was given. Consider using --help."
		);
	}
	if (args.count >= 2) {
		throw ZynkError(
			ZynkErrorType::CLIError,
			"Too many arguments."
		);
	}
}

void CLI::show_help() const {
	std::cout << std::endl << "<----- Zynk Help ----->" << std::endl;
	std::cout << "Zynk - A simple interpreted programming language, written in C++." << std::endl << std::endl;
	std::cout << "Example Usage:\n >> Zynk main.zk\n\n";
	std::cout << "Arguments:\n"
		" --file <path>: Specifies the path to the script file that you want to interpret.\n"
		" --init: Initializes a basic script file template in the current directory.\n"
		" --version: Displays the current version of Zynk interpreter.\n"
		" --help: Displays this help message.\n";
}