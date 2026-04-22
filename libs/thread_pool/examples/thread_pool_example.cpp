#include <iostream>
#include "ThreadPool.hpp"

int main()
{
	ThreadPool pool(4);

	auto future1 = pool.enqueue([](){
				return 42;
			});

	auto future2 = pool.enqueue([](int x, int y){
				return x + y;
			}, 10, 20);

	auto future3 = pool.enqueue([](){
				throw std::runtime_error("OOPS");
				return 0;
			});

	std::cout << "Result1: " << future1.get() << std::endl;
	std::cout << "Result2: " << future2.get() << std::endl;

	try
	{
		future3.get();
	}
	catch(const std::exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return 0;
}
