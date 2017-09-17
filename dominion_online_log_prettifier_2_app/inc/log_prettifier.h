#pragma once

#include <regex>
#include <string>
#include <vector>

namespace dominion_online {

class LogPrettifier {
public:
	void open(const std::string& path);
	void prettify();
	void saveTo(const std::string& path) const;

private:
	void findTurnsAndExtractOutsideTurnsPart();
	void extractCurrentTurn();

	void appendGamePreludeHeader();
	void appendGameNumberAndType();
	void appendPlayerNamesAndLevels();

	void appendCurrentTurnHeader();
	void appendNewLine();

	void appendAllSentencesIn(const std::string& logPart);
	std::vector<std::string> getAllSentencesIn(const std::string& logPart) const;

	void appendPrettifiedSentence(const std::string& unprettySentence);
	std::string prettifySentence(const std::string& sentence) const;

	//Different log parts and iterator for the turns
	std::string log_;
	std::string prettifiedLog_;
	std::string outsideTurnsLog_;
	std::string currentTurnLog_;
	std::sregex_iterator currentTurn_;

	//Regex patterns and regexes
	static const std::string PLAYER_NAME_EXP_PATTERN;
	static const std::string PLAYER_LEVEL_EXP_PATTERN;
	static const std::string TURN_HEADER_EXP_PATTERN;

	static const std::regex PLAYER_NAME_EXP;
	static const std::regex PLAYER_LEVEL_EXP;
	static const std::regex TURN_HEADER_EXP;
};

}
