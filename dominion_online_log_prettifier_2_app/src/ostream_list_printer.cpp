#include "../inc/ostream_list_printer.h"

#include <cassert>

#include <iomanip>
#include <ios>
#include <regex>

namespace stream_utils {

void OStreamListPrinter::setStream(std::ostream* outStream) {
	outStream_ = outStream;
}

void OStreamListPrinter::setItemWidth(size_t itemWidth) {
	itemWidth_ = itemWidth;
}

void OStreamListPrinter::setDescriptionWidth(size_t descriptionWidth) {
	descriptionWidth_ = descriptionWidth;
}

void OStreamListPrinter::printList(const std::list<ListItem>& list) {
	if (outStream_ == nullptr)
		throw std::invalid_argument("The stream to be printed to has not been set.");

	for (const auto& item : list)
		printItem(item);
}

void OStreamListPrinter::printItem(const ListItem& item) {
	printName(item.name);
	printDescription(item.description);
	*outStream_ << "\n";
}

void OStreamListPrinter::printName(const std::string& name) {
	*outStream_ << std::setw(itemWidth_) << std::left << name;
}

void OStreamListPrinter::printDescriptionLine(const std::string& line) {
	*outStream_ << std::setw(descriptionWidth_) << std::left << line << "\n";
}

void OStreamListPrinter::printDescription(const std::string& description) {
	splitDescription(description);
	if (pieces_.size() == 0)
		return;

	printDescriptionLine(pieces_[0]);
	for (size_t i = 1; i < pieces_.size(); ++i) {
		printName("");
		printDescriptionLine(pieces_[i]);
	}
}

void OStreamListPrinter::splitDescription(const std::string& sentence) {
	resetPieces();
	const std::regex WORD_EXP("\\b[[:alnum:].]+[[:punct:]]?([[:s:]]|$)");
	std::sregex_iterator wordIt(sentence.begin(), sentence.end(), WORD_EXP);
	const std::sregex_iterator end;
	for (; wordIt != end; ++wordIt)
		processWord(wordIt->str());
}

void OStreamListPrinter::resetPieces() {
	pieces_.clear();
	pieces_.emplace_back();
	pieceIndex_ = 0;
}

void OStreamListPrinter::processWord(const std::string& word) {
	size_t wordPartBegin = 0;
	while (wordPartBegin < word.length()) {
		wordPart_ = word.substr(wordPartBegin, descriptionWidth_);
		processWordPart();
		wordPartBegin += descriptionWidth_;
	}
}

void OStreamListPrinter::processWordPart() {
	if (wordPartFitsPiece()) {
		pieces_[pieceIndex_].append(wordPart_);
	}
	else {
		pieces_.emplace_back(wordPart_);
		++pieceIndex_;
	}
}

bool OStreamListPrinter::wordPartFitsPiece() const {
	assert(descriptionWidth_ >= pieces_[pieceIndex_].length());
	return descriptionWidth_ - pieces_[pieceIndex_].length() >= wordPart_.length();
}

}
