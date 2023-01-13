#pragma once
#include <vector>
#include <unordered_set>
#include <iostream>


class TextProcessor
{
private:
	std::vector<std::string> split(std::string &&line);
	std::unordered_set<std::string>&  words_;
	static const std::string  delimiters_; ;
public:
	TextProcessor(std::unordered_set<std::string>& result);
	~TextProcessor() = default;
	std::unordered_set<std::string>& getResult();
	void process(std::string&& line);



};