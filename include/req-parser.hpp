#ifndef REQ_PARSER_HPP
#define REQ_PARSER_HPP
#include <memory>
#include <string>
#include "request.hpp"
using Headers     = std::unordered_map<std::string, std::string>;
using QueryParams = std::unordered_map<std::string, std::string>;
/**
 * @class ReqParser
 * @brief Class that servers purpose of parsing raw HTTP request.
 *
 */
class ReqParser {
private:
    std::string              rawHttpRequest;
    std::unique_ptr<Request> req;

public:
    explicit ReqParser();

    /**
     * @brief Takes raw HTTP request as a string, returns Request object
     *
     * @param request raw HTTP request as a string
     * @return Request object.
     */
    std::unique_ptr<Request> parseRequest(std::string& request);

private:
    PathAndType parsePathAndType(std::string& request);

    MethodType parseMethod(std::string& request);

    void parseHeaders(std::string& request, Headers& headers) const;

    bool lineNotEmpty(std::string& line) const;

    void parseQueryParams(std::string& path, QueryParams& queryParams);
};

#endif  // DEBUG
