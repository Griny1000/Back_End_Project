#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <unordered_map>

struct HttpRequest
{
	std::string method;
	std::string path;
	std::string version;
	std::unordered_map<std::string, std::string> headers;
	std::string body;
};


HttpRequest parseHttpRequest(const std::string& raw);
#endif
