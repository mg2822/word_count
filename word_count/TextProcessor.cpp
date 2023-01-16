#include "TextProcessor.h"

using namespace std;

string const TextProcessor::delimiters_ = { '\x20', '\x09', '\x0a', '\x0b', '\x0c', '\x0d' };

std::vector<std::string> TextProcessor::split(std::string&& line)
{
	auto words = std::vector<string>{};
	size_t pos, prev_pos = 0;
	while ((pos = line.find_first_of(delimiters_, prev_pos)) !=
		std::string::npos)
	{
		if (pos > prev_pos)
			words.push_back(line.substr(prev_pos, pos - prev_pos));
		prev_pos = pos + 1;
	}
	if (prev_pos < line.length())
		words.push_back(line.substr(prev_pos, std::string::npos));
	return words;
}

std::unordered_set<std::string>& TextProcessor::getResult()
{
	return words_;
}


void TextProcessor::process(std::string&& line)
{
	auto words = split(move(line));
	for (const auto& w : words)
	{
		if (words_.find(w) == words_.end())
		{
			words_.insert(w);
		}
	}

}

TextProcessor::TextProcessor(std::unordered_set<std::string>& result) : 
	words_(result)
{

}