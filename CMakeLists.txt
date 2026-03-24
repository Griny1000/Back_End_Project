cmake_minimum_required(VERSION 3.15)
project(CppBackEnd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(libs/threadpool)

add_subdirectory(projects/http_server)
add_subdirectory(projects/in_memory_cache)

