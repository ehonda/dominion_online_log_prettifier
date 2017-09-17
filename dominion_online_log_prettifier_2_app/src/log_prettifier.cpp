#include "../inc/log_prettifier.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../inc/string_utils.h"

namespace dominion_online {

void LogPrettifier::open(const std::string& path) {
	std::ifstream logFile(path);
	if (!logFile.is_open())
		throw std::runtime_error("Could not open specified source file.");

	std::stringstream buffer;
	buffer << logFile.rdbuf();
	log_ = buffer.str();
	logFile.close();
}

void LogPrettifier::prettify() {
	findTurnsAndExtractOutsideTurnsPart();

	appendGamePreludeHeader();
	appendNewLine();

	appendAllSentencesIn(outsideTurnsLog_);
	appendNewLine();

	const std::sregex_iterator turnsEnd;
	for (; currentTurn_ != turnsEnd; ++currentTurn_) {
		extractCurrentTurn();

		appendCurrentTurnHeader();
		appendNewLine();

		appendAllSentencesIn(currentTurnLog_);
		appendNewLine();
	}
}

void LogPrettifier::saveTo(const std::string& path) const {
	try {
		std::ofstream targetFile(path);
		targetFile << string_utils::trim(prettifiedLog_);
		targetFile.close();
	}
	catch (const std::exception& e) {
		throw std::runtime_error("Error occured while writing to target file: " + std::string(e.what()));
	}
	
}

void LogPrettifier::findTurnsAndExtractOutsideTurnsPart() {
	currentTurn_ = std::sregex_iterator(
		log_.begin(), log_.end(), TURN_HEADER_EXP);

	const std::sregex_iterator turnsEnd;
	if (currentTurn_ == turnsEnd)
		outsideTurnsLog_ = log_;
	else
		outsideTurnsLog_ = currentTurn_->prefix().str();
}

void LogPrettifier::extractCurrentTurn() {
	auto nextTurn = currentTurn_;
	const std::sregex_iterator turnsEnd;
	size_t end;
	if (++nextTurn != turnsEnd)
		end = nextTurn->position();
	else
		end = std::string::npos;

	const size_t start = currentTurn_->position();
	currentTurnLog_ = log_.substr(start, end - start);
}

void LogPrettifier::appendGamePreludeHeader() {
	appendGameNumberAndType();
	appendNewLine();
	appendPlayerNamesAndLevels();
}

void LogPrettifier::appendGameNumberAndType() {
	const std::regex GAME_NUMBER_EXP("Game #\\d+, (rated|unrated)\\.");
	std::smatch match;
	if (std::regex_search(outsideTurnsLog_, match, GAME_NUMBER_EXP)) {
		prettifiedLog_.append(match.str());
		//Remove game-number sentence from the outside
		//turns log to have it start with its first sentence
		outsideTurnsLog_ = match.suffix();
	}
}

void LogPrettifier::appendPlayerNamesAndLevels() {
	std::sregex_iterator currentPlayer(outsideTurnsLog_.begin(), outsideTurnsLog_.end(), PLAYER_LEVEL_EXP);
	const std::sregex_iterator playersEnd;
	std::string lastPlayerSuffix;
	for (; currentPlayer != playersEnd; ++currentPlayer) {
		appendPrettifiedSentence(currentPlayer->str());
		appendNewLine();
		lastPlayerSuffix = currentPlayer->suffix().str();
	}
	//Remove player-name sentences from the outside
	//turns log to have it start with its first sentence
	if (lastPlayerSuffix.length() > 0)
		outsideTurnsLog_ = lastPlayerSuffix;
}

void LogPrettifier::appendCurrentTurnHeader() {
	std::smatch match;
	if (std::regex_search(currentTurnLog_, match, TURN_HEADER_EXP)) {
		appendPrettifiedSentence(match.str());
		//Remove turn header from the log
		//to have it start with its first sentence
		currentTurnLog_ = match.suffix();
	}
}

void LogPrettifier::appendNewLine() {
	prettifiedLog_.append("\n");
}

void LogPrettifier::appendAllSentencesIn(const std::string& logPart) {
	const auto sentences = getAllSentencesIn(logPart);
	for (const auto& sentence : sentences) {
		appendPrettifiedSentence(sentence);
		appendNewLine();
	}
}

std::vector<std::string> LogPrettifier::getAllSentencesIn(const std::string& logPart) const {
	std::vector<std::string> sentences;
	size_t lastDotPosition = 0;
	const size_t len = logPart.length();
	for (size_t i = 0; i < len; ++i) {
		if (logPart[i] == '.' && (i == len - 1 || logPart[i + 1] == '\n')) {
			sentences.emplace_back(logPart.substr(lastDotPosition, i + 1 - lastDotPosition));
			lastDotPosition = i + 1;
		}
	}
	return sentences;
}

void LogPrettifier::appendPrettifiedSentence(const std::string& unprettySentence) {
	prettifiedLog_.append(prettifySentence(unprettySentence));
}

std::string LogPrettifier::prettifySentence(const std::string& sentence) const {
	const auto replaced = string_utils::replaceAll(sentence, '\n', ' ');
	const auto unnecessaryBlanksRemoved = string_utils::removeBlanksInFrontOfSpecialCharacters(replaced);
	return string_utils::trim(unnecessaryBlanksRemoved);
}

const std::string LogPrettifier::PLAYER_NAME_EXP_PATTERN("[[:alnum:] ._]+");
const std::string LogPrettifier::PLAYER_LEVEL_EXP_PATTERN("^" + PLAYER_NAME_EXP_PATTERN + "\\s*:\\s\\d+\\.*\\d*");
const std::string LogPrettifier::TURN_HEADER_EXP_PATTERN("^Turn \\d+ - " + PLAYER_NAME_EXP_PATTERN);

const std::regex LogPrettifier::PLAYER_NAME_EXP(PLAYER_NAME_EXP_PATTERN);
const std::regex LogPrettifier::PLAYER_LEVEL_EXP(PLAYER_LEVEL_EXP_PATTERN);
const std::regex LogPrettifier::TURN_HEADER_EXP(TURN_HEADER_EXP_PATTERN);

}
