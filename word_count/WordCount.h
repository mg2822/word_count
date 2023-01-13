#pragma once

#include <iostream>
#include "FileReader.h"
#include "TextProcessor.h"

class WordCount
{
private:
	void fillBuffer(vector<vector<string>>& buffer, unique_ptr<FileReader>& reader);
	mutex m_;
public :
	static size_t NUMBER_OF_LINES_IN_BUFFER_ITEM;
	static size_t NUMBER_OF_LINES_IN_ST_BUFFER;
	static size_t NUMBER_OF_ITEMS_IN_BUFFER;
public:
	WordCount() = default;
	unsigned int countWordsInSingleThread(const char* path);
	unsigned int countWords(const char* path);
	

};






