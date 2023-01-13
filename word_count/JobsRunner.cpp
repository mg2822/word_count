#include "JobsRunner.h"
#include "TextProcessor.h"



Job::Job(deque<vector<string>>& threads_queue_ref,
	unordered_set<string>& result_ref,
	condition_variable& cv,
	mutex& m,
	bool& finished) : threads_queue_ref_(threads_queue_ref), result_ref_(result_ref), cv_(cv), m_(m), finished_(finished)
{
}

void Job::operator()()
{
	unique_ptr<TextProcessor> tp = make_unique<TextProcessor>(result_ref_);
	while (true)
	{
		unique_lock<mutex> ul(m_);
		cv_.wait(ul, [this]() {return (!threads_queue_ref_.empty() || finished_); });
		if (finished_)
		{
			break;
		}
		text_to_process_ = threads_queue_ref_.back();
		threads_queue_ref_.pop_back();
		ul.unlock();
		for (auto & line : text_to_process_)
		{
			tp->process(move(line));
		}
	}

}

JobsRunner::JobsRunner(unsigned int numberOfJobs,
					   vector<vector<string>>& buffer,
					   mutex& m,
					   bool& finished) : 
								numberOfJobs_(numberOfJobs), mainThreadMutex_(m),
								file_reader_buffer_ref_(buffer), finished_(finished)
{
	for (unsigned int i = 0; i < numberOfJobs_; ++i)
	{
		results_.emplace_back();
	}
	for (unsigned int i = 0; i < numberOfJobs_; ++i)
	{
		jobs_.emplace_back(threads_queue_, results_[i], cv_, threads_mutex_, finished_);
	}
	for (unsigned int i = 0; i < numberOfJobs_; ++i)
	{
		threads_.emplace_back(std::ref(jobs_[i]));
	}
}

void JobsRunner::run()
{
	while(true)
	{
		{
			lock_guard<mutex> lmg(mainThreadMutex_);
			if (finished_)
			{
				break;
			}
			if (text_chunks_.empty() && !finished_)
			{
				text_chunks_ = move(file_reader_buffer_ref_);
			}

		}
		if (!text_chunks_.empty())
		{
			{
				lock_guard<mutex> ltg(threads_mutex_);
				for (unsigned int i = 0; i < numberOfJobs_; ++i)
				{
					threads_queue_.emplace_back(move(text_chunks_.back()));
					text_chunks_.pop_back();

					if (text_chunks_.empty())
					{
						break;
					}
				}
			}
			if (!threads_queue_.empty())
			{
				cv_.notify_all();
			}			
		}

	}
	if (finished_)
	{
		cv_.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		for (unsigned int i = 0; i < numberOfJobs_; ++i)
		{
			threads_[i].join();
		}
	}
}

unsigned int JobsRunner::getResults()
{
	unordered_set<string> result;
	for (auto& set : results_)
	{
		result.merge(set);
	}
	return static_cast<unsigned int>(result.size());
}
