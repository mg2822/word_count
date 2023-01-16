#pragma once

#include "WordCounter.h"
#include "FileReader.h"
#include <mutex>

using namespace std;

class MultiThreadWordCounter : public WordCounter
{
private:
	mutex m_;
	static size_t NUMBER_OF_LINES_IN_BUFFER_ITEM;
	static size_t NUMBER_OF_ITEMS_IN_BUFFER;
	void fillBuffer(vector<vector<string>>& buffer, unique_ptr<FileReader>& reader);
public:
	virtual ~MultiThreadWordCounter();
	unsigned int countWords(const char* path) override;

};