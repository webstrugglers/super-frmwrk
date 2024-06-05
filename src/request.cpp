/**
 * @file
 * @brief Implements the Request class used for representing HTTP requests
 */

#include "request.hpp"
#include "constants.hpp"

Request::Request(
    const PathAndType&                                  new_path_and_type,
    const std::string&                                  new_body,
    const std::unordered_map<std::string, std::string>& new_headers,
    const std::string&                                  new_path_var,
    const std::unordered_map<std::string, std::string>& new_query_params)
    : path_and_type(new_path_and_type),
      body(new_body),
      headers(new_headers),
      path_var(new_path_var),
      query_params(new_query_params) {}

Request::Request(const Request& req) = default;

PathAndType Request::getPathAndType() const {
    return path_and_type;
}

std::string Request::getBody() const {
    return body;
}

std::unordered_map<std::string, std::string> Request::getHeaders() const {
    return headers;
}

std::unordered_map<std::string, std::string> Request::getQueryParams() const {
    return query_params;
}

std::string Request::getPathVar() const {
    return path_var;
}

MethodType Request::getMethodType() const {
    return path_and_type.getMethodType();
}

std::string Request::getPath() const {
    return path_and_type.getPath();
}

void Request::setPathAndType(const PathAndType& new_path_and_type) {
    this->path_and_type = PathAndType(new_path_and_type);
}

void Request::setBody(const std::string& new_body) {
    this->body = new_body;
}

void Request::setHeaders(
    const std::unordered_map<std::string, std::string>& new_headers) {
    this->headers = new_headers;
}

void Request::setQueryParams(
    const std::unordered_map<std::string, std::string>& new_query_params) {
    this->query_params = new_query_params;
}

void Request::setPathVar(const std::string& new_path_var) {
    this->path_var = new_path_var;
}
