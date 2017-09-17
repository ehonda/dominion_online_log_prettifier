#include "../inc/string_utils.h"

#include <algorithm>
#include <regex>
#include <set>

namespace dominion_online {
namespace string_utils {

std::string removeAll(std::string s, const char& target) {
	const auto newEnd = std::remove_if(s.begin(), s.end(),
		[&target](const char& c) {
		return c == target;
	});
	return std::string(s.begin(), newEnd);
}

std::string replaceAll(std::string s, const char& target, const char& replacement) {
	std::replace_if(s.begin(), s.end(),
		[&target](const char& c) {
		return c == target;
	}, replacement);
	return s;
}

std::string removeBlanksInFrontOfSpecialCharacters(const std::string& s) {
	const std::regex exp("([[:blank:]]+)([,.):])");
	return std::regex_replace(s, exp, "$2");
}

std::string trim(const std::string& s) {
	const std::set<char> whitespace = { ' ', '\t', '\n', '\v', '\f', '\r' };
	const auto stringBegin = std::find_if(s.begin(), s.end(),
		[&whitespace](const char& c) {
		return whitespace.find(c) == whitespace.end();
	});
	if (stringBegin == s.end())
		return "";

	const auto stringEnd = std::find_if(s.rbegin(), s.rend(),
		[&whitespace](const char& c) {
		return whitespace.find(c) == whitespace.end();
	});
	return std::string(stringBegin, stringEnd.base());
}

std::string appendToFileName(const std::string& fileName, const std::string& appendix) {
	const auto fileTypeBegin = fileName.find_last_of(".");
	if (fileTypeBegin != std::string::npos) {
		const auto filePath = fileName.substr(0, fileTypeBegin);
		const auto fileType = fileName.substr(fileTypeBegin);
		return filePath + appendix + fileType;
	}
	else {
		throw std::invalid_argument("No filetype found in the supplied filename.");
	}
}

bool endsWith(const std::string& s, const std::string& endSubstring) {
	if (s.length() < endSubstring.length())
		return false;

	const auto stringEnd = s.substr(s.length() - endSubstring.length());
	if (stringEnd == endSubstring)
		return true;
	else
		return false;
}

}
}
