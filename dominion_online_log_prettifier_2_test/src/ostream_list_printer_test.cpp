#include <sstream>

#include "gtest/gtest.h"

#include "inc/ostream_list_printer.h"

using namespace stream_utils;

namespace dominion_online_test {

class OStreamListPrinterTest : public testing::Test {
protected:
	OStreamListPrinterTest() {
		printer_.setStream(&outStream_);
		printer_.setItemWidth(10);
		printer_.setDescriptionWidth(30);
	}

	void printList(const std::list<OStreamListPrinter::ListItem>& list) {
		printer_.printList(list);
	}

	void expectTextDisplayed(const std::string& expectedText) {
		const auto actualText = outStream_.str();
		EXPECT_EQ(actualText, expectedText);
	}

	std::stringstream outStream_;
	OStreamListPrinter printer_;
};

TEST_F(OStreamListPrinterTest, print_description_with_too_long_word_wrapping_multiple_lines) {
	printer_.setDescriptionWidth(10);
	printList({ {"wrap", "wrapwrapwrapwrapwrapwrap abc"} });

	//ALIGNHELP			"01234567890123456789012345678901234567890123456789
	expectTextDisplayed("wrap      wrapwrapwr\n"
						"          apwrapwrap\n"
						"          wrap abc  \n\n");
}

TEST_F(OStreamListPrinterTest, print_one_item_with_multi_line_description) {
	printList({ {"-o", "This is the overwrite option. It lets you overwrite stuff." } });

	//ALIGNHELP			"01234567890123456789012345678901234567890123456789
	expectTextDisplayed("-o        This is the overwrite option. \n"
						"          It lets you overwrite stuff.  \n\n");
}

TEST_F(OStreamListPrinterTest, print_two_items_separated_by_newline) {
	printList({ {"-o", "This is the overwrite option."}, {"-h", "This is the help option."} });

	//ALIGNHELP			"01234567890123456789012345678901234567890123456789
	expectTextDisplayed("-o        This is the overwrite option. \n"
						"\n"
						"-h        This is the help option.      \n\n");
}

TEST_F(OStreamListPrinterTest, throws_if_called_with_nullptr_stream) {
	printer_.setStream(nullptr);
	EXPECT_THROW(printer_.printList({ {"-o", "Overwrite"} }), std::invalid_argument);
}

}
