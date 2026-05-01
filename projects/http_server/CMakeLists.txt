cmake_minimum_required(VERSION 3.15)
project(HttpServer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(http_server 
	src/main.cpp
	src/HttpRequest.cpp
	src/HttpResponse.cpp
	)

target_link_libraries(http_server PRIVATE ThreadPool  pthread)
target_include_directories(http_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include	
	${CMAKE_CURRENT_SOURCE_DIR}/../../libs/thread_pool/include
)

add_custom_command(TARGET http_server POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_SOURCE_DIR}/static
	$<TARGET_FILE_DIR:http_server>/static
	COMMENT "Copying static files to binary directory"
)
