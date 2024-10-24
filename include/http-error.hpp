#ifndef HTTP_ERROR
#define HTTP_ERROR
#include <string>
#include <utility>
#include "http-status-codes.hpp"
/**
 * @class HttpError
 * @brief Serves purpose of standardizing and abstracting error handling
 *
 */
struct HttpError {
    HttpStatus  status;
    std::string message;
    std::string function_name;
    HttpError(HttpStatus status_code, std::string func_name);
};
#endif
