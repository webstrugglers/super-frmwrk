#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>
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
    std::unordered_map<std::string, std::string> query;
    std::unordered_map<std::string, std::string> path_params;

    Request();
};

#endif  // REQUEST_HPP
