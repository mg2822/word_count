#pragma once
#include "WordCounter.h"


class SingleThreadWordCounter : public WordCounter
{
public:
	static size_t NUMBER_OF_LINES_IN_BUFFER;

public:
	SingleThreadWordCounter() = default;
	virtual unsigned int countWords(const char* path);
};