#include <fstream>
#include <sstream>

#include "gtest/gtest.h"

#include "inc/log_prettifier.h"

#include "../inc/testing_utils.h"

using namespace dominion_online;

namespace dominion_online_test {

class LogPrettifierTest : public testing::Test {
protected:
	//Actual and expected prettified logs directories
	static const std::string TEST_LOGS_DIR;
	static const std::string PRETTIFIED_TEST_LOGS_DIR;
	static const std::string EXPECTED_TEST_LOGS_DIR;

	//Examples to prettify
	static const std::string EXAMPLE_GAME_PRELUDE;
	static const std::string EXAMPLE_GAME_PRELUDE_HEADER;
	static const std::string EXAMPLE_GAME_PRELUDE_SETUP;
	static const std::string EXAMPLE_ONE_TURN;
	static const std::string EXAMPLE_GAME_PRELUDE_AND_ONE_TURN;
	static const std::string EXAMPLE_BLACK_MARKET_BUG;
	static const std::string EXAMPLE_REGEX_STACK_OVERFLOW;
	static const std::string EXAMPLE_REGEX_TURNS_STACK_OVERFLOW;
	static const std::string EXAMPLE_ALREADY_OPENED;

	//###################################################################
	//Begin Technical/Helper methods

	void clearFile(const std::string& path) const
	{
		std::ofstream file;
		file.open(path, std::ios::out | std::ios::trunc);
		file.close();
	}

	std::string prependTestLogsDir(const std::string& fileName) const {
		return TEST_LOGS_DIR + "/" + fileName;
	}

	std::string prependPrettifiedDir(const std::string& fileName) const {
		return PRETTIFIED_TEST_LOGS_DIR + "/" + fileName;
	}

	std::string prependExpectedDir(const std::string& fileName) const {
		return EXPECTED_TEST_LOGS_DIR + "/" + fileName;
	}

	//End Technical/Helper methods
	//###################################################################

	//###################################################################
	//Begin Domain Specific Language

	void runAndVerifyOn(const std::string& example) {
		operateOn(example);
		open();
		logPrettifier_.prettify();
		save();
		compareWithExpected();
	}

	void operateOn(const std::string& example) {
		currentExample_ = example;
	}

	void open() {
		logPrettifier_.open(prependTestLogsDir(currentExample_));
	}

	void save() {
		clearFile(prependPrettifiedDir(currentExample_));
		logPrettifier_.saveTo(prependPrettifiedDir(currentExample_));
	}

	void compareWithExpected() const {
		compareWithExpected(currentExample_);
	}
	
	void compareWithExpected(const std::string& fileName) const {
		std::ifstream fileActual, fileExpected;
		fileActual.open(prependPrettifiedDir(fileName));
		fileExpected.open(prependExpectedDir(fileName));

		const auto actual = testing_utils::readFileContentToString(fileActual);
		const auto expected = testing_utils::readFileContentToString(fileExpected);

		EXPECT_EQ(actual, expected);
		fileActual.close();
		fileExpected.close();
	}

	//End Domain Specific Language
	//###################################################################

	std::string currentExample_;
	LogPrettifier logPrettifier_;
};

const std::string LogPrettifierTest::TEST_LOGS_DIR = "test_logs";
const std::string LogPrettifierTest::PRETTIFIED_TEST_LOGS_DIR = "prettified_test_logs";
const std::string LogPrettifierTest::EXPECTED_TEST_LOGS_DIR = "expected_test_logs";

const std::string LogPrettifierTest::EXAMPLE_GAME_PRELUDE = "example_game_prelude.txt";
const std::string LogPrettifierTest::EXAMPLE_GAME_PRELUDE_HEADER = "example_game_prelude_header.txt";
const std::string LogPrettifierTest::EXAMPLE_GAME_PRELUDE_SETUP = "example_game_prelude_setup.txt";
const std::string LogPrettifierTest::EXAMPLE_ONE_TURN = "example_one_turn.txt";
const std::string LogPrettifierTest::EXAMPLE_GAME_PRELUDE_AND_ONE_TURN = "example_game_prelude_and_one_turn.txt";
const std::string LogPrettifierTest::EXAMPLE_BLACK_MARKET_BUG = "example_black_market_bug.txt";
const std::string LogPrettifierTest::EXAMPLE_REGEX_STACK_OVERFLOW = "example_regex_stack_overflow.txt";
const std::string LogPrettifierTest::EXAMPLE_REGEX_TURNS_STACK_OVERFLOW = "example_regex_turns_stack_overflow.txt";
const std::string LogPrettifierTest::EXAMPLE_ALREADY_OPENED = "example_already_opened.txt";

TEST_F(LogPrettifierTest, prettify_game_prelude_header) {
	runAndVerifyOn(EXAMPLE_GAME_PRELUDE_HEADER);
}

TEST_F(LogPrettifierTest, prettify_game_prelude_setup) {
	runAndVerifyOn(EXAMPLE_GAME_PRELUDE_SETUP);
}

TEST_F(LogPrettifierTest, prettify_full_game_prelude) {
	runAndVerifyOn(EXAMPLE_GAME_PRELUDE);
}

TEST_F(LogPrettifierTest, prettify_one_turn) {
	runAndVerifyOn(EXAMPLE_ONE_TURN);
}

TEST_F(LogPrettifierTest, prettify_prelude_and_one_turn) {
	runAndVerifyOn(EXAMPLE_GAME_PRELUDE_AND_ONE_TURN);
}

TEST_F(LogPrettifierTest, failing_to_open_file_throws) {
	EXPECT_THROW(logPrettifier_.open("FILE_DOES_NOT_EXISTS"), std::runtime_error);
}

//The following tests take a long time to run and are therefore commented out

//TEST_F(LogPrettifierTest, black_market_log_test) {
//	runAndVerifyOn(EXAMPLE_BLACK_MARKET_BUG);
//}
//
//TEST_F(LogPrettifierTest, test_for_sentences_stack_overflow) {
//	runAndVerifyOn(EXAMPLE_REGEX_STACK_OVERFLOW);
//}
//
//TEST_F(LogPrettifierTest, test_for_turns_stack_overflow) {
//	runAndVerifyOn(EXAMPLE_REGEX_TURNS_STACK_OVERFLOW);
//}

}
