cmake_minimum_required(VERSION 3.15)
project(ThreadPool VERSION 0.1.0 LANGUAGES CXX)

enable_testing()

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ThreadPool STATIC
	src/ThreadPool.cpp)
target_include_directories(ThreadPool PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	)


include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
	)
FetchContent_MakeAvailable(googletest)


FetchContent_Declare(
	benchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
	GIT_TAG v1.9.5
	)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

add_executable(thread_pool_tests tests/test_thread_pool.cpp)
target_link_libraries(thread_pool_tests PRIVATE ThreadPool gtest_main)
target_include_directories(thread_pool_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(thread_pool_tests)

add_executable(thread_pool_benchmark bench/bench_thread_pool.cpp)
target_link_libraries(thread_pool_benchmark PRIVATE ThreadPool benchmark::benchmark benchmark::benchmark_main)
target_include_directories(thread_pool_benchmark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/includei)
