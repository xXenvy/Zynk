#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>

struct Arguments {
	std::string file_path;
	bool help;
	bool version;
	bool init;
	std::vector<std::string> extras;

	bool empty() const;
};

class CLI {
public:
	Arguments args{};
	CLI(const std::vector<std::string>& raw_args);
	void checkout() const;
	void show_help() const;
};

#endif // CLI_H
