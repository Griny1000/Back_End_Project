#include "HttpRequest.hpp"
#include <sstream>
#include <iostream>


HttpRequest parseHttpRequest(const std::string& raw)
{
	HttpRequest req;
	std::istringstream stream(raw);
	std::string line;

	if(std::getline(stream, line) && !line.empty() && line.back() == '\r')
	{
		line.pop_back();
		std::istringstream lineStream(line);
		lineStream >> req.method >> req.path >> req.version;
	}

	while(std::getline(stream, line) && line != "\r")
	{
		if(!line.empty() && line.back() == '\r') line.pop_back();
		if(line.empty()) continue;

		size_t colonPos = line.find(':');
		if (colonPos != std::string::npos) 
		{
			std::string key = line.substr(0, colonPos);
			std::string value = line.substr(colonPos + 1);
			size_t first = value.find_first_not_of(" \t");
			if (first != std::string::npos) value = value.substr(first);
			req.headers[key] = value;
		}
	}

	return req;
}
