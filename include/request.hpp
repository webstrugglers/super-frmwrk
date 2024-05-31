#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
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
    PathAndType                        path_and_type;
    std::string                        body;
    std::map<std::string, std::string> headers;
    std::string                        path_var;
    std::map<std::string, std::string> query_params;

     public:
    explicit Request(
        const PathAndType&                        path_and_type = PathAndType(),
        const std::string&                        body          = "",
        const std::map<std::string, std::string>& headers       = {},
        const std::string&                        path_var      = "",
        const std::map<std::string, std::string>& query_params  = {});

    Request(const Request& req);

    PathAndType                        getPathAndType() const;
    std::string                        getBody() const;
    std::map<std::string, std::string> getHeaders() const;
    std::map<std::string, std::string> getQueryParams() const;
    std::string                        getPathVar() const;
    MethodType                         getMethodType() const;
    std::string                        getPath() const;

    void setPathAndType(const PathAndType& path_and_type);
    void setBody(const std::string& body);
    void setHeaders(const std::map<std::string, std::string>& headers);
    void setQueryParams(const std::map<std::string, std::string>& query_params);
    void setPathVar(const std::string& path_var);
};

#endif  // REQUEST_HPP
