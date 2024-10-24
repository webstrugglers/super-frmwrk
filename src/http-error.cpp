#include "http-error.hpp"
#include <utility>
#include "http-status-codes.hpp"
#include "logger.hpp"

HttpError::HttpError(HttpStatus status_code, std::string func_name)
    : status(status_code),
      message(http_status_message[status_code]),
      function_name(std::move(func_name)) {
    if (http_status_code[status] >= 500 && http_status_code[status] <= 511) {
        SafeLogger::log(&"ERROR: "[status]);
        SafeLogger::log("Message: " + message);
        SafeLogger::log("Function name: " + function_name);
    }
}
