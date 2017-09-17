#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace dominion_online_test {
namespace testing_utils {

std::vector<std::string> getArgv(const std::string& commandLine);

std::string readFileContentToString(const std::string& filePath);
std::string readFileContentToString(std::ifstream& file);

}
}
