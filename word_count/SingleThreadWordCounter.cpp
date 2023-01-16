#include "SingleThreadWordCounter.h"
#include "FileReader.h"
#include "TextProcessor.h"

size_t SingleThreadWordCounter::NUMBER_OF_LINES_IN_BUFFER = 128;

unsigned int SingleThreadWordCounter::countWords(const char* path)
{
	unique_ptr<FileReader> reader = make_unique<FileReader>(path);
	std::unordered_set<std::string> result;
	unique_ptr<TextProcessor> tp = make_unique<TextProcessor>(result);
	vector<string> buffer;
	while (true)
	{
		auto buffer = reader->read(NUMBER_OF_LINES_IN_BUFFER);
		if (buffer.empty())
		{
			break;
		}
		for (auto& l : buffer)
		{
			tp->process(move(l));
		}
	}
	return static_cast<unsigned int>(tp->getResult().size());
}