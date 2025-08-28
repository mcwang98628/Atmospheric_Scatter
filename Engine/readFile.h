#pragma once
#include <vector>
#include <string>

static class FileReader {
public:
	static std::vector<char> readFile(const std::string& filename);
};
