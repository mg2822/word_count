#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_set>
#include <string>
#include <vector>
#include <deque>

#include "TextProcessor.h"

using namespace std;

class Job
{
private:
	unordered_set<string>& result_ref_;
	deque<vector<string>>& threads_queue_ref_;
	vector<string> text_to_process_;
	condition_variable& cv_;
	mutex& m_;
	bool& finished_;


public:
	Job(deque<vector<string>>& threads_queue_ref,
		unordered_set<string>& result_ref,
		condition_variable& cv,
		mutex& m,
		bool & finished );
	~Job() = default;
	void operator()();
};

class JobsRunner
{
private:
	deque<vector<string>> threads_queue_;
	vector<vector<string>>& file_reader_buffer_ref_;
	vector<vector<string>> text_chunks_;
	vector<std::unordered_set<string>> results_;
	condition_variable cv_;
	mutex& mainThreadMutex_;
	mutex threads_mutex_;
	vector<Job> jobs_;
	vector<std::thread> threads_;
	unsigned int numberOfJobs_;
	bool& finished_;

public:
	JobsRunner(unsigned int numberOfJobs,
		       vector<vector<string>>& buffer,
		       mutex & m,
			   bool& finished);
	unsigned int getResults();
	void run();



};