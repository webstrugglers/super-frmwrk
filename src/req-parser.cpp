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
}
Request RequestParser::parseRequest(const std::string& request) {
    Request            parsedRequest;
    std::istringstream rawRequest(request);
    PathAndType        pathAndType = parsePathAndType(rawRequest);
    parsedRequest.setPathAndType(pathAndType);
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;
    parseHeaders(rawRequest, headers);
    parsedRequest.setHeaders(headers);
    // provera jel body prazan
    if (std::stoi(headers["Content-Length"]) > 0) {
        parsedRequest.setBody(parseBody(rawRequest));
    } else {
        parsedRequest.setBody("");
    }
    parseQueryParams(pathAndType.path, params);
    parsedRequest.setQueryParams(params);
    return parsedRequest;
}
PathAndType RequestParser::parsePathAndType(std::istringstream& request) const {
    std::string data;
    std::string http_version;
    getline(request, data, ' ');
    MethodType method = parseMethod(data);
    getline(request, data, ' ');
    PathAndType pathAndType(data, method);
    getline(request, data, '\n');
    http_version = data;
    return pathAndType;
}
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
}
void RequestParser::parseHeaders(std::istringstream& request,
                                 Headers&            headers) const {
    std::string line;
    std::string headervalue;
    std::string headername;
    // proverava da li smo na kraju headera tako sto prepoznaje da je poslednja
    // linija empty ili je samo ispunjena whitespaceovima
    while (getline(request, line) && lineNotEmpty(line)) {
        unsigned long int separator_index = line.find(':');
        if (line.find(' ') > separator_index + 1) {
            continue;
        }
        headername  = line.substr(0, separator_index);
        headervalue = line.substr(separator_index + 2, line.length() - 1);
        headers[headername] = headervalue;
    }
}
bool RequestParser::lineNotEmpty(std::string& line) const {
    return !line.empty() &&
           (line.find_first_not_of("\r\n \t") != std::string::npos);
}
std::string RequestParser::parseBody(std::istringstream& request) const {
    std::string line;
    getline(request, line, '\0');
    return line;
}

void RequestParser::parseQueryParams(const std::string& path,
                                     QueryParams&       queryParams) {
    std::istringstream pathStream(path);
    std::string        line;
    if (path.find('?') != std::string::npos &&
        path.find('=') != std::string::npos) {
        std::cout << "Ima param" << std::endl;
    } else {
        std::cout << "Nema param";
    }
    getline(pathStream, line, '?');
    std::string name;
    std::string value;
    while (getline(pathStream, line, '=') && !line.empty()) {
        name = line;
        getline(pathStream, line, '&');
        value             = line;
        queryParams[name] = value;
    }
}
