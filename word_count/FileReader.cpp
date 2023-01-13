#include "FileReader.h"



FileReader::FileReader(const char* path) : filePath(path)
{
	isFilePathExist = fs::exists(filePath);
	if (!isFilePathExist)
	{
		throw runtime_error("file not exist.");
	}
	inFile.open(filePath.native(), ios::in);

	if (!inFile.is_open())
	{
		throw runtime_error("Unable to open input file.");
	}
}

FileReader::~FileReader()
{
	if (inFile.is_open() && (inFile.good() || inFile.eof()))
	{
		inFile.close();
	}
}

vector<string> FileReader::read(unsigned int numOfLines)
{
	vector<string> result{};
	if (inFile.is_open() )
	{
			string line;
			while(result.size() < numOfLines
				  && inFile.good() 
				  && !inFile.eof())
			{
				if (getline(inFile, line))
				{
					if (line.length() > 0)
					{
						result.emplace_back(move(line));
					}
				}
			}
	}
	return result;
}

 unsigned int FileReader::getFileSize(const char* path)
{
	return static_cast<unsigned int>(fs::file_size(fs::path(path)));
}
