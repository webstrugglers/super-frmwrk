#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>
#include "constants.hpp"
#include "path-and-type.hpp"

/**
 * @class Request
 * @brief A class to represent an HTTP request.
 *
 * This class encapsulates the details of an HTTP request, including the method,
 * URI, headers, and body. It provides methods to access and manipulate these
 * details.
 */
class Request {
private:
    PathAndType                                  path_and_type;
    std::string                                  body;
    std::unordered_map<std::string, std::string> headers;
    std::string                                  path_var;
    std::unordered_map<std::string, std::string> query_params;

public:
    explicit Request(
        const PathAndType& new_path_and_type = PathAndType(),
        std::string        new_body          = "",
        const std::unordered_map<std::string, std::string>& new_headers  = {},
        std::string                                         new_path_var = "",
        const std::unordered_map<std::string, std::string>& new_query_params =
            {});

    PathAndType                                  getPathAndType() const;
    std::string                                  getBody() const;
    std::unordered_map<std::string, std::string> getHeaders() const;
    std::unordered_map<std::string, std::string> getQueryParams() const;
    std::string                                  getPathVar() const;
    MethodType                                   getMethodType() const;
    std::string                                  getPath() const;

    void setPathAndType(const PathAndType& new_path_and_type);
    void setBody(std::string new_body);
    void setHeaders(
        const std::unordered_map<std::string, std::string>& new_headers);
    void setQueryParams(
        const std::unordered_map<std::string, std::string>& new_query_params);
    void setPathVar(const std::string& new_path_var);
};

#endif  // REQUEST_HPP
