/**
 * @file
 * @brief Implements the Request class used for representing HTTP requests
 */

#include "request.hpp"

Request::Request(
    const PathAndType&                                  new_path_and_type,
    std::string                                         new_body,
    const std::unordered_map<std::string, std::string>& new_headers,
    std::string                                         new_path_var,
    const std::unordered_map<std::string, std::string>& new_query_params)
    : path_and_type(new_path_and_type.path, new_path_and_type.method_type),
      body(std::move(new_body)),
      headers(new_headers),
      path_var(std::move(new_path_var)),
      query_params(new_query_params) {}
