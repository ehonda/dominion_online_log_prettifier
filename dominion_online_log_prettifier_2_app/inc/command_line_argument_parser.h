#pragma once

#include <string>
#include <vector>

namespace dominion_online {

class CommandLineArgumentParser {
public:
	static const std::string OVERWRITE_OPTION;
	static const std::string HELP_OPTION;

	void parse(const std::vector<std::string>& argv);

	std::string getSource() const noexcept;
	std::string getDestination() const noexcept;
	bool getOverwriteOption() const noexcept;
	bool getHelpOption() const noexcept;

private:
	void reset();

	void parseArgument(const std::string& argument);
	void parseOption(const std::string& argument);
	void parseFilePath(const std::string& argument);

	bool isOption(const std::string& argument) const;
	bool isPathToTxtFile(const std::string& argument) const;

	std::string source_;
	std::string destination_;
	bool overwriteOption_ = false;
	bool helpOption_ = false;
};

}
