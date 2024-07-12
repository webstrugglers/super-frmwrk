#include "req-parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "constants.hpp"
#include "path-and-type.hpp"
#include "request.hpp"

RequestParser::RequestParser(const std::string& rawRequest)
    : rawHttpRequest(rawRequest) {}

void RequestParser::printRawHttpRequest(const std::string& request) {
    std::cout << "RAW REQUEST:\n" << request << std::endl;
};
Request RequestParser::parseRequest(const std::string& request) {
    Request            parsedRequest;
    std::istringstream rawRequest(request);
    std::string        data;
    std::string        path;
    std::string        http_version;
    MethodType         methodType = MethodType::HTTP_GET;
    getline(rawRequest, data, ' ');
    // Potencijalno da ovo odvojim u funkciju koja ce da
    // postavi methodtype da malo bude citljivije?
    if (data == "PUT") {
        methodType = MethodType::HTTP_PUT;
    } else if (data == "POST") {
        methodType = MethodType::HTTP_POST;
    } else if (data == "DELETE") {
        methodType = MethodType::HTTP_DELETE;
    } else if (data == "HEAD") {
        methodType = MethodType::HTTP_HEAD;
    } else if (data == "TRACE") {
        methodType = MethodType::HTTP_TRACE;
    } else if (data == "PATCH") {
        methodType = MethodType::HTTP_PATCH;
    } else if (data == "OPTIONS") {
        methodType = MethodType::HTTP_OPTIONS;
    } else if (data == "CONNECT") {
        methodType = MethodType::HTTP_CONNECT;
    }
    getline(rawRequest, data, ' ');
    PathAndType pathAndType(data, methodType);
    getline(rawRequest, data, '\n');
    http_version = data;
    parsedRequest.setPathAndType(pathAndType);
    return parsedRequest;
};
