#include <iostream>
#include "SingleThreadWordCounter.h"
#include "MultiThreadWordCounter.h"
#include "FileReader.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong number of parameters.";
		return 1;
	}	

	try
	{
		if (FileReader::getFileSize(argv[1]) <= 1024 * 1024)
		{
			unique_ptr<SingleThreadWordCounter> wc = make_unique<SingleThreadWordCounter>();
			cout << wc->countWords(argv[1]);
		}
		else
		{
			unique_ptr<MultiThreadWordCounter> wc = make_unique<MultiThreadWordCounter>();
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