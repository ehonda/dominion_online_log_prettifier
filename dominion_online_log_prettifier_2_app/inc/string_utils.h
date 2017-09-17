#pragma once

#include <string>

namespace dominion_online {
namespace string_utils {

std::string removeAll(std::string s, const char& target);
std::string replaceAll(std::string s, const char& target, const char& replacement);
//Removes blank (' ' '\t') characters in front of the following special characters: '.' ',' ')' ':'
std::string removeBlanksInFrontOfSpecialCharacters(const std::string& s);
std::string trim(const std::string& s);

std::string appendToFileName(const std::string& fileName, const std::string& appendix);
bool endsWith(const std::string& s, const std::string& endSubstring);

}
}
