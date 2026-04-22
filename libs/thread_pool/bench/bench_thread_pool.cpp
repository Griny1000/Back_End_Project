#include "ThreadPool.hpp"
#include <benchmark/benchmark.h>
#include <atomic>
#include <chrono>


static void BM_ThreadPool_SimpleTask(benchmark::State& state)
{
	ThreadPool pool(std::thread::hardware_concurrency());
	std::atomic<int> counter{0};


	for(auto _ : state)
	{
		pool.enqueue([&counter]()
				{
					counter++;
				});
	}


	while(counter != state.iterations())
	{
		std::this_thread::yield();
	}
}

BENCHMARK(BM_ThreadPool_SimpleTask)->Iterations(100000);

static void BM_ThreadPool_SleepyTask(benchmark::State& state)
{
	ThreadPool pool(std::thread::hardware_concurrency());

	for(auto _ : state)
	{
		auto future = pool.enqueue([](){
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				return 42;
				});

		int res = future.get();
		benchmark::DoNotOptimize(res);
	}
}
BENCHMARK(BM_ThreadPool_SleepyTask)->Iterations(100);
BENCHMARK_MAIN();
