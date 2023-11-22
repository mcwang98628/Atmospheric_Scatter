#include <vector>
#include <iostream>
#include <fstream>

static class FileReader {
public:
	static std::vector<char> readFile(const std::string& filename);
};
