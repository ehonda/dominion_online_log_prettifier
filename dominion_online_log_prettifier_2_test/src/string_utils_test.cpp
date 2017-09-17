#include "gtest/gtest.h"

#include "inc/string_utils.h"

using namespace dominion_online;
using namespace string_utils;

namespace dominion_online_test {

class StringUtilsTest : public testing::Test {
};

TEST_F(StringUtilsTest, remove_all_new_lines) {
	const std::string original = "\n\nRemove\n all \n\n\nnew lines\n.\n";
	const std::string expected = "Remove all new lines.";
	const auto transformed = removeAll(original, '\n');
	EXPECT_EQ(expected, transformed);
}

TEST_F(StringUtilsTest, replace_all_new_lines_by_spaces) {
	const std::string original = "\nReplace all\n\nnew  lines by \nspaces.\n";
	const std::string expected = " Replace all  new  lines by  spaces. ";
	const auto transformed = replaceAll(original, '\n', ' ');
	EXPECT_EQ(expected, transformed);
}

TEST_F(StringUtilsTest, remove_spaces_in_front_of_special_characters) {
	const std::string original = "Remove spaces    .";
	const std::string expected = "Remove spaces.";
	const auto transformed = removeBlanksInFrontOfSpecialCharacters(original);
	EXPECT_EQ(expected, transformed);
}

TEST_F(StringUtilsTest, trim) {
	const std::string original = "  \t\n  \tTrim this sentence.  \v\f\r\t";
	const std::string expected = "Trim this sentence.";
	const auto transformed = trim(original);
	EXPECT_EQ(expected, transformed);
}

TEST_F(StringUtilsTest, append_string_to_filename) {
	const std::string fileName = "file.txt";
	const std::string expected = "file_prettified.txt";
	const auto transformed = appendToFileName(fileName, "_prettified");
	EXPECT_EQ(expected, transformed);
}

TEST_F(StringUtilsTest, supplying_non_filename_to_append_function_throws) {
	EXPECT_THROW(appendToFileName("abc", "_invalid"), std::invalid_argument);
}

TEST_F(StringUtilsTest, ends_with) {
	const std::string txtFile = "path/to/file.txt";
	EXPECT_TRUE(endsWith(txtFile, ".txt"));

	const std::string pdfFile = "path/to/file.pdf";
	EXPECT_FALSE(endsWith(pdfFile, ".txt"));
}

}
