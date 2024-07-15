/**
 * @file
 * @brief Implements the Response class used for representing HTTP requests
 */

#include "response.hpp"
#include <cstring>
#include "constants.hpp"

Response::Response() : status_code(OK) {}

Response& Response::status(HttpStatus code) {
    status_code = code;
    return *this;
}

Response& Response::body(std::string str) {
    this->data.append(str);
    return *this;
}

const char* Response::pSendData() const {
    return this->data.c_str();
}
