#ifndef REQ_PARSER_HPP
#define REQ_PARSER_HPP
#include <memory>
#include <string>
#include "constants.hpp"
#include "path-and-type.hpp"
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
    Request parseRequest(std::string& request);
    /**
     * @brief Independent method for parsing headers from HTTP Request.(Can be
     * called anywhere in code and it will parse them).
     *
     * @param request Raw HTTP Request;
     * @return Pointer to request.
     */
    std::unique_ptr<Request> parseHeaderSection(const std::string& str);

private:
    PathAndType parsePathAndType(std::string& request);

    MethodType parseMethod(std::string& request);

    void parseHeaders(std::string& request, Headers& headers) const;

    std::string parseBody(std::string& request) const;

    bool lineNotEmpty(std::string& line) const;

    void parseQueryParams(std::string& path, QueryParams& queryParams);
};
#endif
