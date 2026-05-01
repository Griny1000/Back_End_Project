#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ThreadPool.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <filesystem>

const int PORT = 8080;
const int BUFFER_SIZE = 4096;
const std::string STATIC_ROOT = "./static/";

void handleClient(int clientFd)
{
	char buffer[BUFFER_SIZE] = {0};
	int bytesRead = read(clientFd, buffer, BUFFER_SIZE - 1);
	if(bytesRead <= 0)
	{
		close(clientFd);	
	}
	std::string raw(buffer, bytesRead);
	HttpRequest req = parseHttpRequest(raw);

	std::cout << "Thread " << std::this_thread::get_id()
		    << " -> " << req.method << " " << req.path << std::endl;

	if(req.path.find("/static/") == 0)
	{
		std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
		for (const auto& entry : std::filesystem::directory_iterator("./static"))
			    std::cout << "Static file found: " << entry.path() << std::endl;
		std::string relative = req.path.substr(8);
		std::string fullPath = STATIC_ROOT + relative;
		if(sendFile(clientFd, fullPath))
		{
			close(clientFd);
			return;
		}
		else
		{
			std::string error = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
			send(clientFd, error.c_str(), error.size(), 0);
			close(clientFd);
			return;
		}
	}

	std::string response = buildResponse(req);
	send(clientFd, response.c_str(), response.size(), 0);
	close(clientFd);
}

int main()
{
	ThreadPool pool(4);

	int serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverFd < 0)
	{
		std::cerr << "Failed to create socket\n";
		return 1;
	}

	int opt = 1;
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);


	if(bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		std::cerr << "Bind failed\n";
		close(serverFd);
		return 1;
	}

	if(listen(serverFd, 3) < 0)
	{
		std::cerr << "Listen failed\n";
		close(serverFd);
		return 1;
	}

	std::cout << "Multithreaded server listening on port " << PORT << std::endl;

	while(true)
	{
		socklen_t addrlen = sizeof(address);
		int clientFd = accept(serverFd, (struct sockaddr*)&address, &addrlen);
		if(clientFd < 0)
		{
			std::cerr << "Accept failed\n";
			continue;
		}

		std::cout << "New connection accepted. Dispatching to thread pool.\n";

		pool.enqueue([clientFd](){
				handleClient(clientFd);
				});
	}

	close(serverFd);
	return 0;
}
