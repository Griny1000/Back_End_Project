#include <gtest/gtest.h>
#include "ThreadPool.hpp"
#include <atomic>
#include <chrono>

TEST(ThreadPoolTest, SingleTask)
{
	ThreadPool pool(1);
	std::atomic<bool> executed{false};
	pool.enqueue([&executed]()
			{
				executed = true;
			}
			);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_TRUE(executed);
}

TEST(ThreadPoolTest, MultipleTasks)
{
	ThreadPool pool(2);
	std::atomic<int> counter{0};
	const int numTasks = 10;
	for(int  i = 0; i < numTasks; ++i)
	{
		pool.enqueue([&counter]()
				{
					counter++;
				});
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	EXPECT_EQ(counter, numTasks);
}

TEST(TreadPoolTest, TasksRun)
{
	ThreadPool pool(4);
	std::atomic<int> concurrentCount{0};
	const int numTasks = 8;
	std::atomic<int> maxConcurrent{0};

	for(int i = 0; i < numTasks; ++i)
	{
		pool.enqueue([&]()
				{
					int cur = ++concurrentCount;
					int prev = maxConcurrent.load();
				while(cur > prev && !maxConcurrent.compare_exchange_weak(prev, cur))
				{
					prev = maxConcurrent.load();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				--concurrentCount;	
				});
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	EXPECT_LE(maxConcurrent.load(), 4);
}

TEST(ThreadPoolTest, FutureInt)
{
	ThreadPool pool(1);
	auto fut = pool.enqueue([](){return 123;});
	EXPECT_EQ(fut.get(), 123);
}

TEST(ThreadPoolTest, FutureWithArgs)
{
	ThreadPool pool(1);
	auto fut = pool.enqueue([](int x, int y){return x * y;}, 6, 7);
	EXPECT_EQ(fut.get(), 42);
}

TEST(ThreadPoolTest, FutureException)
{
	ThreadPool pool(1);
	auto fut = pool.enqueue([](){
				throw std::string("test exception");
			});
	EXPECT_THROW(fut.get(), std::string);
}

TEST(ThreadPoolTest, MultipleFutures)
{
	ThreadPool pool(4);
	std::vector<std::future<int>> futures;
	for(int i = 0; i < 10; ++i)
	{
		futures.emplace_back(pool.enqueue([i](){return i * i;}));
	}
	for(int i = 0; i < 10; ++i)
	{
		EXPECT_EQ(futures[i].get(), i * i);
	}
}
