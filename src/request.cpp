#include "request.hpp"

Request::Request(const PathAndType&                        path_and_type,
                 const std::string&                        body,
                 const std::map<std::string, std::string>& headers,
                 const std::string&                        path_var,
                 const std::map<std::string, std::string>& query_params)
    : path_and_type(path_and_type),
      body(body),
      headers(headers),
      path_var(path_var),
      query_params(query_params) {}

Request::Request(const Request& req) = default;

PathAndType Request::getPathAndType() const {
    return path_and_type;
}

std::string Request::getBody() const {
    return body;
}

std::map<std::string, std::string> Request::getHeaders() const {
    return headers;
}

std::map<std::string, std::string> Request::getQueryParams() const {
    return query_params;
}

std::string Request::getPathVar() const {
    return path_var;
}

void Request::setPathAndType(const PathAndType& path_and_type) {
    this->path_and_type = PathAndType(path_and_type);
}

void Request::setBody(const std::string& body) {
    this->body = body;
}

void Request::setHeaders(const std::map<std::string, std::string>& headers) {
    this->headers = headers;
}

void Request::setQueryParams(
    const std::map<std::string, std::string>& query_params) {
    this->query_params = query_params;
}

void Request::setPathVar(const std::string& path_var) {
    this->path_var = path_var;
}
