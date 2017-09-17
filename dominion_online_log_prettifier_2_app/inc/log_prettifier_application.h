#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "command_line_argument_parser.h"

namespace dominion_online {

class LogPrettifierApplication {
public:
	LogPrettifierApplication(std::ostream& outStream);

	void run(int argc, char* argv[]);
	void run(const std::vector<std::string>& argv);

private:
	void testConsistencyOfArguments() const;
	bool helpAndAdditionalArgumentsSpecified() const noexcept;
	bool noHelpAndNoSourceSpecified() const noexcept;

	void displayError(const std::exception& e) const;
	void displayHelp() const;
	void displayArgumentList() const;

	void prettifyAndSave() const;
	std::string getSaveTarget() const;
	bool destinationSpecified() const;

	std::ostream& outStream_;
	CommandLineArgumentParser parser_;
};

namespace log_prettifier_application_strings {

const std::string HELP_HINT{
	"Type \"logprettifier -h\" to display help." };
const std::string HELP_HEADER{
	"You can control the logprettifier application by passing one or several of the following commandline arguments:\n" };
const std::string HELP_EXAMPLE{
	"An example with source and destination file specified would be:\n"
	"logprettifier c:/dominion_logs/black_market_game.txt c:/pretty_dominion_logs/black_market_game.txt" };

const std::string HELP_ITEM_FILEPATH_NAME{
	"path/to/file.txt" };
const std::string HELP_ITEM_FILEPATH_DESCRIPTION{
	"This represents a path to a file containing either a full dominion online log or only parts of it."
	" At least one filepath has to be passed to the application, this is the log to be worked on. If two"
	" filepaths are passed the second is interpreted as the destination to save the prettified log to. Only"
	" filepaths ending in .txt are accepted. Two is the maximum number of filepaths that can be specified."
	" If the path you want to specify contains spaces make sure to surround it by quotation marks."};

const std::string HELP_ITEM_OVERWRITE_OPTION_NAME{
	"-o" };
const std::string HELP_ITEM_OVERWRITE_OPTION_DESCRIPTION{
	"This option tells the application to overwrite the file that the prettified log should be saved to,"
	" if it is present. If you specify only the path to the source file, this option will lead to the"
	" unprettified log being overwritten by its prettified version. If you specify a source and destination"
	" file, the destination file will be overwritten if it is present. If this option is not activated, the"
	" the application will make sure no files (except those ending in _prettified.txt) are overwritten by"
	" appending _prettified to the filename of the prettified log in case saving it to that destination would"
	" cause a file to be overwritten." };

const std::string HELP_ITEM_HELP_OPTION_NAME{
	"-h" };
const std::string HELP_ITEM_HELP_OPTION_DESCRIPTION{
	"Specifying this option shows this helptext. It can not be specified in combination with any other arguments." };

const std::string ERROR_OCCURED{
	"Error occured: " };
const std::string ERROR_DESCRIPTION_NO_SOURCE{
	"No path to the logfile to be processed was specified." };
const std::string ERROR_DESCRIPTION_HELP_AND_ADDITIONAL_ARGUMENTS{
	"The help option can not be specified alongside other parameters." };

}

}
