#pragma once

class WordCounter
{
public :
	static size_t NUMBER_OF_LINES_IN_BUFFER_ITEM;
	static size_t NUMBER_OF_ITEMS_IN_BUFFER;
public:
	virtual ~WordCounter();
	virtual unsigned int countWords(const char* path) = 0;


};






