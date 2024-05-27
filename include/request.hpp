#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <map>
#include <string>
#include "path-and-type.hpp"

// TODO: NEEDS OPTIONAL PATH VAR AND QUERY PARAMS

class Request {
     private:
    PathAndType                        path_and_type;
    std::string                        body;
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string> query_params;
    std::string                        path_var;

     public:
    Request();

    Request(const PathAndType& path_and_type                  = PathAndType(),
            const std::string& body                           = "",
            const std::map<std::string, std::string>& headers = {},
            const std::map<std::string, std::string>& query_params = {},
            const std::string&                        path_var     = "");

    Request(const Request& req);

    PathAndType                        getPathAndType() const;
    std::string                        getBody() const;
    std::map<std::string, std::string> getHeaders() const;
    std::map<std::string, std::string> getQueryParams() const;
    std::string                        getPathVar() const;

    void setPathAndType(const PathAndType& path_and_type);
    void setBody(const std::string& body);
    void setHeaders(const std::map<std::string, std::string>& headers);
    void setQueryParams(const std::map<std::string, std::string>& query_params);
    void setPathVar(const std::string& path_var);
};

#endif  // REQUEST_HPP
