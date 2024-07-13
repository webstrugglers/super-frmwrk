#ifndef REQ_PARSER_HPP
#define REQ_PARSER_HPP
#include <sstream>
#include <string>
#include "constants.hpp"
#include "path-and-type.hpp"
#include "request.hpp"
using Headers     = std::unordered_map<std::string, std::string>;
using QueryParams = std::unordered_map<std::string, std::string>;
class RequestParser {
private:
    std::string rawHttpRequest;

public:
    explicit RequestParser(const std::string& request);

    static void printRawHttpRequest(const std::string& request);

    Request parseRequest(const std::string& request);

    PathAndType parsePathAndType(std::istringstream& request) const;

    [[nodiscard]] MethodType parseMethod(const std::string& request) const;

    void parseHeaders(std::istringstream& request, Headers& headers) const;

    std::string parseBody(std::istringstream& request) const;

    bool lineNotEmpty(std::string& line) const;

    void parseQueryParams(const std::string& path, QueryParams& queryParams);
};
#endif
