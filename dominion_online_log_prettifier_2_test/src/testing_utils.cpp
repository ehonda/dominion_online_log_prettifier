#include "../inc/testing_utils.h"

#include <fstream>
#include <sstream>

namespace dominion_online_test {
namespace testing_utils {

std::vector<std::string> getArgv(const std::string & commandLine)
{
	std::vector<std::string> argv{ "program_name_representation" };
	size_t argBegin = 0, argEnd = 0;
	bool finished = false;
	while (!finished) {
		argEnd = commandLine.find_first_of(" \t", argBegin);
		argv.emplace_back(commandLine.substr(argBegin, argEnd - argBegin));
		argBegin = argEnd + 1;

		if (argEnd == std::string::npos)
			finished = true;
	}
	return argv;
}

std::string readFileContentToString(const std::string& filePath)
{
	std::string content;
	std::ifstream file(filePath);
	if (file.is_open()) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		content = buffer.str();
		file.close();
	}
	return content;
}

std::string readFileContentToString(std::ifstream& file)
{
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

}
}
