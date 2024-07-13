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
    PathAndType        pathAndType = parsePathAndType(rawRequest);
    parsedRequest.setPathAndType(pathAndType);
    std::unordered_map<std::string, std::string> headers;
    parseHeaders(rawRequest, headers);
    parsedRequest.setHeaders(headers);

    return parsedRequest;
};
PathAndType RequestParser::parsePathAndType(std::istringstream& request) const {
    std::string data;
    std::string path;
    std::string http_version;
    getline(request, data, ' ');
    MethodType method = parseMethod(data);
    // Potencijalno da ovo odvojim u funkciju koja ce da
    // postavi methodtype da malo bude citljivije?
    getline(request, data, ' ');
    PathAndType pathAndType(data, method);
    getline(request, data, '\n');
    http_version = data;
    return pathAndType;
};
MethodType RequestParser::parseMethod(const std::string& request) const {
    if (request == "GET") {
        return MethodType::HTTP_GET;
    } else if (request == "PUT") {
        return MethodType::HTTP_PUT;
    } else if (request == "POST") {
        return MethodType::HTTP_POST;
    } else if (request == "DELETE") {
        return MethodType::HTTP_DELETE;
    } else if (request == "HEAD") {
        return MethodType::HTTP_HEAD;
    } else if (request == "TRACE") {
        return MethodType::HTTP_TRACE;
    } else if (request == "PATCH") {
        return MethodType::HTTP_PATCH;
    } else if (request == "OPTIONS") {
        return MethodType::HTTP_OPTIONS;
    } else if (request == "CONNECT") {
        return MethodType::HTTP_CONNECT;
    } else {
        return MethodType::HTTP_GET;
    }
};
Headers RequestParser::parseHeaders(std::istringstream& request,
                                    Headers&            headers) const {
    std::string line;
    std::string headervalue;
    std::string headername;
    while (getline(request, line) && !line.empty()) {
        unsigned long int separator_index = line.find(':');
        if (line.find(' ') > separator_index + 1) {
            continue;
        }
        headername  = line.substr(0, separator_index);
        headervalue = line.substr(separator_index + 2, line.length() - 1);
        headers[headername] = headervalue;
    }
    return headers;
}
