/**
 * @file
 * @brief Parses raw HTTP request and returns a Request object.
 */

#include "req-parser.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include "constants.hpp"
#include "path-and-type.hpp"
#include "request.hpp"

RequestParser::RequestParser(std::string& request) : rawHttpRequest(request) {}

/// Parses raw HTTP Request directly from original request string
/// and puts parsed data into field of Request object
Request RequestParser::parseRequest(std::string& request) {
    Request     parsedRequest;
    PathAndType pathAndType = parsePathAndType(request);
    parsedRequest.setPathAndType(pathAndType);
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;
    parseHeaders(request, headers);

    parsedRequest.setHeaders(headers);
    std::string body = std::move(request);
    parsedRequest.setBody(body);

    parseQueryParams(pathAndType.path, params);
    parsedRequest.setQueryParams(params);
    return parsedRequest;
}
/// Parses path and type of HTTP Request into PathAndType enum
PathAndType RequestParser::parsePathAndType(std::string& request) {
    size_t      pos        = request.find(' ');
    std::string data       = request.substr(0, pos);
    MethodType  methodType = parseMethod(data);

    request.erase(0, pos + 1);
    pos = request.find(' ');
    PathAndType pathAndType(request.substr(0, pos), methodType);

    request.erase(0, pos + 1);
    pos = request.find('\r');
    request.erase(0, pos + 2);

    // ako zatreba HTTP version on je ovde
    return pathAndType;
}
/// Parses only method type of HTTP Request into MethodType enum
MethodType RequestParser::parseMethod(std::string& request) {
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
/// Parses headers from HTTP Request into unordered_map
void RequestParser::parseHeaders(std::string& request, Headers& headers) const {
    size_t      pos             = 0;
    size_t      end_pos         = 0;
    size_t      separator_index = 0;
    std::string headername;
    std::string headervalue = "alo";
    while (((end_pos = request.find('\n', pos)) != std::string::npos)) {
        separator_index = request.find(':', pos);
        if (separator_index == std::string::npos || separator_index > end_pos) {
            break;
        }
        headername = request.substr(pos, separator_index - pos);
        headervalue =
            request.substr(separator_index + 2, end_pos - separator_index - 3);
        headers[headername] = headervalue;
        pos                 = end_pos + 1;
    }
    request.erase(0, end_pos + 1);
}
/// Checks if line that is currently being parsed is not empty
bool RequestParser::lineNotEmpty(std::string& line) const {
    return !line.empty() &&
           (line.find_first_not_of("\r\n \t") != std::string::npos);
}
/// Parses queryParams of HTTP Request into unordered_map
void RequestParser::parseQueryParams(std::string& path,
                                     QueryParams& queryParams) {
    size_t pos = path.find('?');
    if (pos == std::string::npos || path.find('=') == std::string::npos) {
    } else {
        size_t end_pos         = 0;
        size_t separator_index = 0;
        path.erase(0, pos + 1);
        std::string name;
        std::string value;
        pos = 0;

        while (pos < path.length()) {
            end_pos = path.find('&', pos);
            if (end_pos == std::string::npos) {
                end_pos = path.length();
            }
            separator_index = path.find('=', pos);
            name            = path.substr(pos, separator_index - pos);
            value =
                path.substr(separator_index + 1, end_pos - separator_index - 1);
            queryParams[name] = value;
            pos               = end_pos + 1;
        }
        path.erase(0, end_pos);
    }
}
