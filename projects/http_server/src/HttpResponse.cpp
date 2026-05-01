#include "HttpResponse.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>


std::string getMimeType(const std::string& path)
{
	if(path.find(".html") != std::string::npos) return "text/html";
	if(path.find(".css") != std::string::npos) return "text/css";
	if(path.find(".js") != std::string::npos) return "application/javascript";
	if(path.find(".jpg") != std::string::npos || path.find(".jpeg") != std::string::npos) return "image/jpeg";
	if(path.find(".png") != std::string::npos) return "image/png";
	return "application/octet-stream";
}

bool sendFile(int clientFd, const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if(!file.is_open()) return false;

	struct stat st;
	if(stat(filePath.c_str(), &st) != 0) return false;
	size_t fileSize = st.st_size;

	std::string mime = getMimeType(filePath);
	std::ostringstream header;
	header << "HTTP/1.1 200 OK\r\n"
	       << "Content-type: " << mime << "\r\n"
	       << "Content-Length: " << fileSize << "\r\n"
	       << "Connection: close\r\n"
	       << "\r\n";
	std::string headerStr = header.str();
	send(clientFd, headerStr.c_str(), headerStr.size(), 0);

	char buffer[8192];
	while(file.read(buffer, sizeof(buffer)))
	{
		send(clientFd, buffer, file.gcount(), 0);
	}
	if(file.gcount() > 0)
	{
		send(clientFd, buffer, file.gcount(), 0);
	}
	return true;
}

std::string buildResponse(const HttpRequest& req, const std::string& staticRoot)
{
	if(req.method != "GET")
	{
		return "HTTP/1.1 405 Method Not Allowed\r\n"
		       "Content-Type: text/plain\r\n"
		       "Content-Length: 18\r\n"
		       "\r\n"
		       "Method Not Allowed";
	}

	if(req.path == "/")
	{
		return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
			"<html><body><h1>Welcome</h1><a href='/static/index.html'>Static page</a></body></html>";
	}

	return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
}
