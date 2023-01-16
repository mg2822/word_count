
#include "MultiThreadWordCounter.h"
#include "FileReader.h"
#include "JobsRunner.h"
#include "TextProcessor.h"

size_t MultiThreadWordCounter::NUMBER_OF_LINES_IN_BUFFER_ITEM = 128;
size_t MultiThreadWordCounter::NUMBER_OF_ITEMS_IN_BUFFER = 128;

MultiThreadWordCounter::~MultiThreadWordCounter()
{
}

void MultiThreadWordCounter::fillBuffer(vector<vector<string>>& buffer, unique_ptr<FileReader>& reader)
{
	for (unsigned int i = 0; i < NUMBER_OF_ITEMS_IN_BUFFER; ++i)
	{
		auto chunk = reader->read(NUMBER_OF_LINES_IN_BUFFER_ITEM);
		if (!chunk.empty())
		{
			buffer.emplace_back(move(chunk));
		}
		else
		{
			break;
		}
	}
}

unsigned int MultiThreadWordCounter::countWords(const char* path)
{
	bool finished = false;
	auto numberOfThreads = thread::hardware_concurrency();

	unique_ptr<FileReader> reader = make_unique<FileReader>(path);
	vector<vector<string>> buffer;
	fillBuffer(buffer, reader);

	// initialize JobRunner and launch threads
	unique_ptr<JobsRunner> jr = make_unique<JobsRunner>(numberOfThreads, buffer, m_, finished);
	thread jrt([&]() { jr->run(); });

	while (true)
	{
		{
			lock_guard<mutex> lg(m_);
			if (buffer.empty())
			{
				fillBuffer(buffer, reader);
				if (buffer.empty())
				{
					finished = true;
					break;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	jrt.join();
	return static_cast<unsigned int>(jr->getResults());
}