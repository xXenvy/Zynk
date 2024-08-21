#ifndef CLI_H
#define CLI_H

#include <string>
#include <vector>

struct Arguments {
	Arguments(const size_t count, const std::string file_path = "", bool help = 0, bool version = 0, bool init = 0) :
		count(count), file_path(file_path), help(help), version(version), init(init) {};
	bool empty() const;

	size_t count;
	std::string file_path;

	bool help = false;
	bool version = false;
	bool init = false;
};

class CLI {
public:
	CLI(const std::vector<std::string>& raw_args);
	Arguments args;

	void checkout() const;
	void show_help() const;
};

#endif // CLI_H
