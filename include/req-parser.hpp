#ifndef REQ_PARSER_HPP
#define REQ_PARSER_HPP
#include <string>
#include "constants.hpp"
#include "path-and-type.hpp"
#include "request.hpp"
using Headers     = std::unordered_map<std::string, std::string>;
using QueryParams = std::unordered_map<std::string, std::string>;
/**
 * @class RequestParser
 * @brief Class that servers purpose of parsing raw HTTP request.
 *
 */
class RequestParser {
private:
    std::string rawHttpRequest;

public:
    explicit RequestParser(std::string& request);

    /**
     * @brief Takes raw HTTP request as a string, returns Request object
     *
     * @param request raw HTTP request as a string
     * @return Request object.
     */
    Request parseRequest(std::string& request);

private:
    PathAndType parsePathAndType(std::string& request);

    MethodType parseMethod(std::string& request);

    void parseHeaders(std::string& request, Headers& headers) const;

    std::string parseBody(std::string& request) const;

    bool lineNotEmpty(std::string& line) const;

    void parseQueryParams(std::string& path, QueryParams& queryParams);
};
#endif
