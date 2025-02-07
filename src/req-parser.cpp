/**
 * @file
 * @brief Parses raw HTTP request and returns a Request object.
 */

#include "req-parser.hpp"
#include <algorithm>
#include <memory>
#include <string>
#include "constants.hpp"
#include "request.hpp"

namespace {
// trim from start (in place)
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}
}  // namespace

ReqParser::ReqParser() : req(std::make_unique<Request>()) {}

/// Parses raw HTTP Request directly from original request string
/// and puts parsed data into field of Request object
std::unique_ptr<Request> ReqParser::parseRequest(std::string& request) {
    PathAndType pathAndType  = parsePathAndType(request);
    this->req->path_and_type = pathAndType;

    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> params;
    parseHeaders(request, headers);
    this->req->headers = headers;

    parseQueryParams(pathAndType.path, params);
    this->req->query = params;

    return std::move(this->req);
}

/// Parses path and type of HTTP Request into PathAndType enum
PathAndType ReqParser::parsePathAndType(std::string& request) {
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
MethodType ReqParser::parseMethod(std::string& request) {
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
void ReqParser::parseHeaders(std::string&    request,
                             super::Headers& headers) const {
    size_t      pos       = 0;
    size_t      end_pos   = 0;
    size_t      separator = 0;
    std::string headername;
    std::string headervalue = "alo";
    while ((end_pos = request.find('\n', pos)) != std::string::npos) {
        separator = request.find(':', pos);
        if (separator == std::string::npos || separator > end_pos) {
            break;
        }

        headername = request.substr(pos, separator - pos);
        trim(headername);
        headervalue = request.substr(separator + 1, end_pos - separator - 1);
        trim(headervalue);

        headers.insert({headername, headervalue});
        pos = end_pos + 1;
    }
    request.erase(0, end_pos + 1);
}

/// Checks if line that is currently being parsed is not empty
bool ReqParser::lineNotEmpty(std::string& line) const {
    return !line.empty() &&
           (line.find_first_not_of("\r\n \t") != std::string::npos);
}

/// Parses queryParams of HTTP Request into unordered_map
void ReqParser::parseQueryParams(std::string& path, QueryParams& queryParams) {
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
