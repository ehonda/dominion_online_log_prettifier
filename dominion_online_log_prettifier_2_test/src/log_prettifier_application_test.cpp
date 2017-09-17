#include <filesystem>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"

#include "inc/log_prettifier_application.h"
#include "inc/string_utils.h"

#include "../inc/testing_utils.h"

using namespace dominion_online;
using namespace log_prettifier_application_strings;

namespace dominion_online_test {

class LogPrettifierApplicationTest : public testing::Test {
protected:
	static const std::string ORIGINAL_LOG;
	static const std::string PRETTIFIED_LOG;
	static const std::string DUMMY_LOG;

	static const std::string SOURCE_LOG_PATH;
	static const std::string SOURCE_APPENDED_LOG_PATH;
	static const std::string DESTINATION_LOG_PATH;
	static const std::string DESTINATION_APPENDED_LOG_PATH;

	LogPrettifierApplicationTest()
		: app_(outStream_) {
		removeAllLogFiles();
	}

	void run(const std::string& commandLine) {
		createSourceLog();
		app_.run(testing_utils::getArgv(commandLine));
	}

	void removeAllLogFiles() const {
		std::experimental::filesystem::remove(SOURCE_LOG_PATH);
		std::experimental::filesystem::remove(SOURCE_APPENDED_LOG_PATH);
		std::experimental::filesystem::remove(DESTINATION_LOG_PATH);
		std::experimental::filesystem::remove(DESTINATION_APPENDED_LOG_PATH);
	}

	void createSourceLog() const {
		std::ofstream source;
		source.open(SOURCE_LOG_PATH, std::ios::out | std::ios::trunc);
		source << ORIGINAL_LOG;
		source.close();
	}

	void createDummyDestinationLog() const {
		std::ofstream destination;
		destination.open(DESTINATION_LOG_PATH, std::ios::out | std::ios::trunc);
		destination << DUMMY_LOG;
		destination.close();
	}

	//----------------------------------------------------------------------------------------
	//Functions for checking the content of the different logs

	void expectOriginalSourceLog() const {
		expectLogInFile(ORIGINAL_LOG, SOURCE_LOG_PATH);
	}

	void expectPrettifiedSourceLog() const {
		expectLogInFile(PRETTIFIED_LOG, SOURCE_LOG_PATH);
	}

	void expectPrettifiedSourceAppendedLog() const {
		expectLogInFile(PRETTIFIED_LOG, SOURCE_APPENDED_LOG_PATH);
	}

	void expectPrettifiedDestinationLog() const {
		expectLogInFile(PRETTIFIED_LOG, DESTINATION_LOG_PATH);
	}

	void expectDummyDestinationLog() const {
		expectLogInFile(DUMMY_LOG, DESTINATION_LOG_PATH);
	}

	void expectPrettifiedDestinationAppendedLog() const {
		expectLogInFile(PRETTIFIED_LOG, DESTINATION_APPENDED_LOG_PATH);
	}

	void expectLogInFile(const std::string& expectedLog, const std::string& filePath) const {
		const auto actualLog = testing_utils::readFileContentToString(filePath);
		EXPECT_EQ(actualLog, expectedLog);
	}

	//----------------------------------------------------------------------------------------

	void expectTextDisplayed(const std::string& expectedText) const {
		const auto actualText = outStream_.str();
		EXPECT_EQ(actualText, expectedText);
	}

	void expectErrorAndHelpHintDisplayed(const std::string& errorMessage) const {
		expectTextDisplayed(ERROR_OCCURED + errorMessage + "\n" + HELP_HINT + "\n");
	}

	void expectHelpDisplayed() const {
		const auto helpText = string_utils::trim(outStream_.str());
		const auto helpHeader = helpText.substr(0, HELP_HEADER.length());
		EXPECT_EQ(helpHeader, HELP_HEADER);

		const auto helpExample = helpText.substr(helpText.length() - HELP_EXAMPLE.length());
		EXPECT_EQ(helpExample, HELP_EXAMPLE);
	}

	std::ostringstream outStream_;
	LogPrettifierApplication app_;
};

const std::string LogPrettifierApplicationTest::ORIGINAL_LOG(	"Turn 1 - E.Honda\n"
																"E\n"
																"plays 4 Coppers\n"
																".\n");
const std::string LogPrettifierApplicationTest::PRETTIFIED_LOG(	"Turn 1 - E.Honda\n"
																"E plays 4 Coppers.");
const std::string LogPrettifierApplicationTest::DUMMY_LOG("DUMMYLOG");

const std::string LogPrettifierApplicationTest::SOURCE_LOG_PATH("../dominion_online_log_prettifier_2_test/app_test_logs/source.txt");
const std::string LogPrettifierApplicationTest::SOURCE_APPENDED_LOG_PATH("../dominion_online_log_prettifier_2_test/app_test_logs/source_prettified.txt");
const std::string LogPrettifierApplicationTest::DESTINATION_LOG_PATH("../dominion_online_log_prettifier_2_test/app_test_logs/destination.txt");
const std::string LogPrettifierApplicationTest::DESTINATION_APPENDED_LOG_PATH("../dominion_online_log_prettifier_2_test/app_test_logs/destination_prettified.txt");

TEST_F(LogPrettifierApplicationTest, test_argc_argv_run) {
	const int argc = 2;
	char** argv = new char*[argc];
	argv[0] = new char[30]{ "program_name_representation" };
	argv[1] = new char[SOURCE_LOG_PATH.length() + 1];
	strcpy_s(argv[1], SOURCE_LOG_PATH.length() + 1, SOURCE_LOG_PATH.c_str());

	createSourceLog();
	app_.run(argc, argv);

	expectOriginalSourceLog();
	expectPrettifiedSourceAppendedLog();

	delete[] argv[0];
	delete[] argv[1];
	delete[] argv;
}

TEST_F(LogPrettifierApplicationTest, specifying_no_source_displays_help_hint) {
	run("-o");
	expectErrorAndHelpHintDisplayed(ERROR_DESCRIPTION_NO_SOURCE);
}

TEST_F(LogPrettifierApplicationTest, specifying_help_along_other_options_displays_help_hint) {
	run("logfilepath.txt -h");
	expectErrorAndHelpHintDisplayed(ERROR_DESCRIPTION_HELP_AND_ADDITIONAL_ARGUMENTS);
}

TEST_F(LogPrettifierApplicationTest, display_help) {
	run("-h");
	expectHelpDisplayed();
}

TEST_F(LogPrettifierApplicationTest, test_text_displayed) {
	run(SOURCE_LOG_PATH);
	expectTextDisplayed("Prettifying " + SOURCE_LOG_PATH + "\n"
						"Saving to " + SOURCE_APPENDED_LOG_PATH + "\n");
}

TEST_F(LogPrettifierApplicationTest, source_and_no_overwrite_specified) {
	run(SOURCE_LOG_PATH);

	expectOriginalSourceLog();
	expectPrettifiedSourceAppendedLog();
}

TEST_F(LogPrettifierApplicationTest, source_and_overwrite_specified) {
	run(SOURCE_LOG_PATH + " -o");

	expectPrettifiedSourceLog();
}

TEST_F(LogPrettifierApplicationTest, source_and_destination_specified_no_overwrite_and_dest_does_not_exist) {
	run(SOURCE_LOG_PATH + " " + DESTINATION_LOG_PATH);

	expectOriginalSourceLog();
	expectPrettifiedDestinationLog();
}

TEST_F(LogPrettifierApplicationTest, source_and_destination_specified_no_overwrite_and_dest_does_exist) {
	createDummyDestinationLog();
	run(SOURCE_LOG_PATH + " " + DESTINATION_LOG_PATH);

	expectOriginalSourceLog();
	expectDummyDestinationLog();
	expectPrettifiedDestinationAppendedLog();
}

TEST_F(LogPrettifierApplicationTest, source_destination_and_overwrite_specified_and_dest_does_not_exist) {
	run(SOURCE_LOG_PATH + " " + DESTINATION_LOG_PATH + " -o");

	expectOriginalSourceLog();
	expectPrettifiedDestinationLog();
}

TEST_F(LogPrettifierApplicationTest, source_destination_and_overwrite_specified_and_dest_does_exist) {
	createDummyDestinationLog();
	run(SOURCE_LOG_PATH + " " + DESTINATION_LOG_PATH + " -o");

	expectOriginalSourceLog();
	expectPrettifiedDestinationLog();
}

}
