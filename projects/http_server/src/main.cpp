#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


const int PORT = 8000;
const int BUFFER_SIZE = 4096;

int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
	{
		std::cerr << "Failed to create socket\n";
		return 1;
	}

	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);


	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		std::cerr << "Bind failed\n";
		close(server_fd);
		return 1;
	}

	if(listen(server_fd, 3) < 0)
	{
		std::cerr << "Listen failed\n";
		close(server_fd);
		return 1;
	}

	std::cout << "Server listening on port " << PORT << std::endl;

	while(true)
	{
		socklen_t addrlen = sizeof(address);
		int client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
		if(client_fd < 0)
		{
			std::cerr << "Accept failed\n";
			continue;
		}

		std::cout << "New connection accepted\n";

		char buffer[BUFFER_SIZE] = {0};
		int bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
		if(bytes_read > 0)
		{
			std::cout << "----- Received HTTP request -----\n";
			std::cout << buffer << std::endl;
			std::cout << "---------------------------------\n";
		}

		std::string response =
			"HTTP/1.1 200 OK\r\n"
			"Conect-Type: text/plain\r\n"
			"Content-Length: 13\r\n"
			"\r\n"
			"Hello, world!";

		send(client_fd, response.c_str(), response.size(), 0);
		close(client_fd);
		std::cout << "Connection closed\n";
	}

	close(server_fd);
	return 0;
}
