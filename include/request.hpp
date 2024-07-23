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
struct Request {
    PathAndType                                  path_and_type;
    std::string                                  body;
    std::unordered_map<std::string, std::string> headers;
    std::string                                  path_var;
    std::unordered_map<std::string, std::string> query_params;

    explicit Request(
        const PathAndType& new_path_and_type = PathAndType(),
        std::string        new_body          = "",
        const std::unordered_map<std::string, std::string>& new_headers  = {},
        std::string                                         new_path_var = "",
        const std::unordered_map<std::string, std::string>& new_query_params =
            {});
};

#endif  // REQUEST_HPP
