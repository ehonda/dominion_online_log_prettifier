#pragma once

#include <list>
#include <ostream>
#include <string>
#include <vector>

namespace stream_utils {

class OStreamListPrinter {
public:
	struct ListItem {
		std::string name;
		std::string description;
	};

	void setStream(std::ostream* outStream);
	void setItemWidth(size_t itemWidth);
	void setDescriptionWidth(size_t descriptionWidth);

	void printList(const std::list<ListItem>& list);

private:
	void printItem(const ListItem& item);
	void printName(const std::string& name);
	void printDescriptionLine(const std::string& line);

	void printDescription(const std::string& description);
	void splitDescription(const std::string& sentence);
	void resetPieces();

	void processWord(const std::string& word);
	void processWordPart();
	bool wordPartFitsPiece() const;

	std::vector<std::string> pieces_;
	size_t pieceIndex_;
	std::string wordPart_;

	std::ostream* outStream_ = nullptr;
	size_t itemWidth_ = 10;
	size_t descriptionWidth_ = 70;
};

}
