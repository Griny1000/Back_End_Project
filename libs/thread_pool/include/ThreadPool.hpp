#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <type_traits>

class ThreadPool
{
public:
	//Constructor : Create the threads
	explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
	//Destructor : stop threads and wait thear exit
	~ThreadPool();

	//Close copy
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	//Add task (function without arguments)
	void enqueue(std::function<void()> task);

	template<typename F, typename... Args>
	auto enqueue(F&& f, Args&&... args)-> std::future<typename std::invoke_result_t<F, Args...>>
	{
		using ReturnType = typename std::invoke_result_t<F, Args...>;

		auto task = std::make_shared<std::packaged_task<ReturnType()>>(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

		std::future<ReturnType> result = task->get_future();
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			if(m_Stop)
			{
				throw std::runtime_error("enqueue on stopped ThreadPool");
			}
			m_Tasks.emplace([task](){(*task)();});
		}
		m_Condition.notify_one();

		return result;
	}
private:
	std::vector<std::thread> m_Workers;
	std::queue<std::function<void()>> m_Tasks;
	std::mutex m_queueMutex;
	std::condition_variable m_Condition;
	std::atomic<bool> m_Stop;
};
#endif
