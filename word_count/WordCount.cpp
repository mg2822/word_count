
#include "WordCount.h"
#include "JobsRunner.h"

using namespace std;

size_t WordCount::NUMBER_OF_LINES_IN_BUFFER_ITEM = 128;
size_t WordCount::NUMBER_OF_LINES_IN_ST_BUFFER = 128;
size_t WordCount::NUMBER_OF_ITEMS_IN_BUFFER = 128;


void WordCount::fillBuffer(vector<vector<string>>& buffer, unique_ptr<FileReader>& reader)
{	
	for(unsigned int i = 0; i < NUMBER_OF_ITEMS_IN_BUFFER; ++i)
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

unsigned int WordCount::countWordsInSingleThread(const char * path)
{
	unique_ptr<FileReader> reader = make_unique<FileReader>(path);
	std::unordered_set<std::string> result;
	unique_ptr<TextProcessor> tp = make_unique<TextProcessor>(result);
	vector<string> buffer;
	while (true)
	{
		auto buffer = reader->read(NUMBER_OF_LINES_IN_ST_BUFFER);
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

unsigned int WordCount::countWords(const char* path)
{
	bool finished = false;
	auto numberOfThreads = thread::hardware_concurrency();
	
	unique_ptr<FileReader> reader = make_unique<FileReader>(path);
	vector<vector<string>> buffer;
	fillBuffer(buffer, reader);

	// initialize JobRunner and launch threads
	unique_ptr<JobsRunner> jr = make_unique<JobsRunner>(numberOfThreads, buffer, m_, finished);
	thread jrt([&](){ jr->run(); });

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


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong number of parameters.";
		return 1;
	}

	unique_ptr<WordCount> wc = make_unique<WordCount>();

	try
	{
		if(FileReader::getFileSize(argv[1]) <= 1024 * 1024)
		{

			cout << wc->countWordsInSingleThread(argv[1]);
			return 0;
		}
		else
		{
			// multi threaded version
			cout << wc->countWords(argv[1]);
		}
	}
	catch (exception& e)
	{
		std::cout << "Caught exception: " << e.what();
		return 2;
	}	
	return 0;
}
 