/**
 * @file
 * @brief Implements the Response class used for representing HTTP requests
 */

#include "response.hpp"

Response::Response() = default;

Response& Response::status(HttpStatus code) {
    status_code = code;
    return *this;
}
