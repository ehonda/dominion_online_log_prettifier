#include "../inc/command_line_argument_parser.h"

#include <stdexcept>

#include "../inc/string_utils.h"

namespace dominion_online {

const std::string CommandLineArgumentParser::OVERWRITE_OPTION{ "-o" };
const std::string CommandLineArgumentParser::HELP_OPTION{ "-h" };

void CommandLineArgumentParser::parse(const std::vector<std::string>& argv) {
	reset();
	//Skip argv[0] since it contains a representation of the
	//program name and not an argument passed from the user
	for (size_t i = 1; i < argv.size(); ++i)
		parseArgument(argv[i]);
}

std::string CommandLineArgumentParser::getSource() const noexcept {
	return source_;
}

std::string CommandLineArgumentParser::getDestination() const noexcept {
	return destination_;
}

bool CommandLineArgumentParser::getOverwriteOption() const noexcept {
	return overwriteOption_;
}

bool CommandLineArgumentParser::getHelpOption() const noexcept {
	return helpOption_;
}

void CommandLineArgumentParser::reset() {
	source_ = "";
	destination_ = "";
	overwriteOption_ = false;
	helpOption_ = false;
}

void CommandLineArgumentParser::parseArgument(const std::string& argument) {
	if (argument.empty())
		throw std::invalid_argument("Empty argument-string supplied.");

	if (isOption(argument))
		parseOption(argument);
	else if (isPathToTxtFile(argument))
		parseFilePath(argument);
	else
		throw std::invalid_argument("One of the arguments supplied is neither an option nor a path to a .txt file.");
}

void CommandLineArgumentParser::parseOption(const std::string& argument) {
	if (argument == OVERWRITE_OPTION)
		overwriteOption_ = true;
	else if (argument == HELP_OPTION)
		helpOption_ = true;
	else
		throw std::invalid_argument("Unrecognized option specified.");
}

void CommandLineArgumentParser::parseFilePath(const std::string& argument) {
	if (source_.empty())
		source_ = argument;
	else if (destination_.empty())
		destination_ = argument;
	else
		throw std::invalid_argument("Too many non-option parameters supplied.");
}

bool CommandLineArgumentParser::isOption(const std::string& argument) const {
	return argument.length() > 0 && argument[0] == '-';
}

bool CommandLineArgumentParser::isPathToTxtFile(const std::string& argument) const {
	return string_utils::endsWith(argument, ".txt");
}

}
