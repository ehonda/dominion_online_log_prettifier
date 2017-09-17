#include "../inc/log_prettifier_application.h"

#include <filesystem>
#include <stdexcept>

#include "../inc/log_prettifier.h"
#include "../inc/ostream_list_printer.h"
#include "../inc/string_utils.h"

namespace dominion_online {

LogPrettifierApplication::LogPrettifierApplication(std::ostream& outStream)
	: outStream_(outStream) {
}

void LogPrettifierApplication::run(int argc, char* argv[]) {
	std::vector<std::string> arguments;
	for (int i = 0; i < argc; ++i)
		arguments.emplace_back(argv[i]);
	run(arguments);
}

void LogPrettifierApplication::run(const std::vector<std::string>& argv) {
	try {
		parser_.parse(argv);
		testConsistencyOfArguments();

		if (parser_.getHelpOption()) {
			displayHelp();
			return;
		}

		prettifyAndSave();
	}
	catch (const std::exception& e) {
		displayError(e);
		return;
	}
}

void LogPrettifierApplication::testConsistencyOfArguments() const {
	if (helpAndAdditionalArgumentsSpecified())
		throw std::invalid_argument(log_prettifier_application_strings::ERROR_DESCRIPTION_HELP_AND_ADDITIONAL_ARGUMENTS);

	if (noHelpAndNoSourceSpecified())
		throw std::invalid_argument(log_prettifier_application_strings::ERROR_DESCRIPTION_NO_SOURCE);
}

bool LogPrettifierApplication::helpAndAdditionalArgumentsSpecified() const noexcept {
	bool help = parser_.getHelpOption();
	bool source = !parser_.getSource().empty();
	bool destination = !parser_.getSource().empty();
	bool overwrite = parser_.getOverwriteOption();
	return help && (source || destination || overwrite);
}

bool LogPrettifierApplication::noHelpAndNoSourceSpecified() const noexcept {
	bool noHelp = !parser_.getHelpOption();
	bool noSource = parser_.getSource().empty();
	return noHelp && noSource;
}

void LogPrettifierApplication::displayError(const std::exception& e) const {
	outStream_ << log_prettifier_application_strings::ERROR_OCCURED << e.what() << "\n";
	outStream_ << log_prettifier_application_strings::HELP_HINT << "\n";
}

void LogPrettifierApplication::displayHelp() const {
	outStream_ << log_prettifier_application_strings::HELP_HEADER << "\n";
	displayArgumentList();
	outStream_ << log_prettifier_application_strings::HELP_EXAMPLE << "\n";
}

void LogPrettifierApplication::displayArgumentList() const {
	stream_utils::OStreamListPrinter listPrinter;
	listPrinter.setStream(&outStream_);
	listPrinter.setItemWidth(20);
	listPrinter.setDescriptionWidth(60);

	stream_utils::OStreamListPrinter::ListItem filePathItem, overwriteOptionItem, helpOptionItem;
	filePathItem.name = log_prettifier_application_strings::HELP_ITEM_FILEPATH_NAME;
	filePathItem.description = log_prettifier_application_strings::HELP_ITEM_FILEPATH_DESCRIPTION;

	overwriteOptionItem.name = log_prettifier_application_strings::HELP_ITEM_OVERWRITE_OPTION_NAME;
	overwriteOptionItem.description = log_prettifier_application_strings::HELP_ITEM_OVERWRITE_OPTION_DESCRIPTION;

	helpOptionItem.name = log_prettifier_application_strings::HELP_ITEM_HELP_OPTION_NAME;
	helpOptionItem.description = log_prettifier_application_strings::HELP_ITEM_HELP_OPTION_DESCRIPTION;

	listPrinter.printList({ filePathItem, overwriteOptionItem, helpOptionItem });
}

void LogPrettifierApplication::prettifyAndSave() const {
	LogPrettifier prettifier;

	prettifier.open(parser_.getSource());
	outStream_ << "Prettifying " << parser_.getSource() << "\n";

	prettifier.prettify();

	const auto target = getSaveTarget();
	prettifier.saveTo(target);
	outStream_ << "Saving to " << target << "\n";
}

std::string LogPrettifierApplication::getSaveTarget() const {
	std::string target;
	if (destinationSpecified())
		target = parser_.getDestination();
	else
		target = parser_.getSource();

	if (std::experimental::filesystem::exists(target) && !parser_.getOverwriteOption())
		target = string_utils::appendToFileName(target, "_prettified");

	return target;
}

bool LogPrettifierApplication::destinationSpecified() const {
	return !parser_.getDestination().empty();
}

}
