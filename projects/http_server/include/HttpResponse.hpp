#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include "HttpRequest.hpp"

std::string buildResponse(const HttpRequest& req, const std::string& staticRoot = "./static/");
bool sendFile(int clientFd, const std::string& filePath);

#endif
