#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "inc/command_line_argument_parser.h"

#include "../inc/testing_utils.h"

using namespace dominion_online;

namespace dominion_online_test {

class CommandLineArgumentParserTest : public testing::Test {
protected:
	void parse(const std::string& commandLine) {
		auto argv = testing_utils::getArgv(commandLine);
		parser_.parse(argv);
	}

	void expectSource(const std::string& expectedSource) const {
		const auto source = parser_.getSource();
		EXPECT_EQ(source, expectedSource);
	}

	void expectDestination(const std::string& expectedDestination) const {
		const auto destination = parser_.getDestination();
		EXPECT_EQ(destination, expectedDestination);
	}

	void expectOverwriteOption(bool expectedOverwriteOption) const {
		const bool overwriteOption = parser_.getOverwriteOption();
		EXPECT_EQ(overwriteOption, expectedOverwriteOption);
	}

	void expectHelpOption(bool expectedHelpOption) const {
		const bool helpOption = parser_.getHelpOption();
		EXPECT_EQ(helpOption, expectedHelpOption);
	}

	CommandLineArgumentParser parser_;
	std::vector<std::string> argv;
};

TEST_F(CommandLineArgumentParserTest, parse_source_and_destination) {
	parse("original.txt prettified.txt");

	expectSource("original.txt");
	expectDestination("prettified.txt");
	expectOverwriteOption(false);
	expectHelpOption(false);
}

TEST_F(CommandLineArgumentParserTest, parse_source_destination_and_overwrite) {
	parse("original.txt prettified.txt -o");

	expectSource("original.txt");
	expectDestination("prettified.txt");
	expectOverwriteOption(true);
	expectHelpOption(false);
}

TEST_F(CommandLineArgumentParserTest, parse_source_without_dest_and_overwrite) {
	parse("original.txt");

	expectSource("original.txt");
	expectDestination("");
	expectOverwriteOption(false);
	expectHelpOption(false);
}

TEST_F(CommandLineArgumentParserTest, parse_source_with_overwrite_option) {
	parse("original.txt -o");

	expectSource("original.txt");
	expectDestination("");
	expectOverwriteOption(true);
	expectHelpOption(false);
}

TEST_F(CommandLineArgumentParserTest, test_help_option) {
	parse("-h");

	expectSource("");
	expectDestination("");
	expectOverwriteOption(false);
	expectHelpOption(true);
}

TEST_F(CommandLineArgumentParserTest, throws_if_unrecognized_option_supplied) {
	EXPECT_THROW(parse("filepath1.txt -unrecognized_option"), std::invalid_argument);
}

TEST_F(CommandLineArgumentParserTest, throws_if_empty_argument_string_supplied) {
	EXPECT_THROW(parse(""), std::invalid_argument);
}

TEST_F(CommandLineArgumentParserTest, throws_if_argument_other_than_option_or_path_to_txt_supplied) {
	EXPECT_THROW(parse("path/to/file.pdf"), std::invalid_argument);
}

}
