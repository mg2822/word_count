#pragma once
#include <filesystem>
#include <vector>
#include <fstream>

namespace fs = std::filesystem;
using namespace std;

class FileReader
{
private:
		fs::path filePath;
		bool isFilePathExist;
		ifstream inFile;

public:
	FileReader(const char* path);
	virtual ~FileReader();
	vector<string>  read(unsigned int numOfLines);
	static unsigned int getFileSize(const char * path);
};