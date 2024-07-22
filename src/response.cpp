/**
 * @file
 * @brief Implements the Response class used for representing HTTP requests
 */

#include "response.hpp"
#include <string>
#include "http-status-codes.hpp"

Response::Response()
    : http_version("HTTP/1.1"),
      status_code(NOT_FOUND),
      status_message(http_status_message[NOT_FOUND]) {}

Response& Response::status(HttpStatus code) {
    this->status_code    = code;
    this->status_message = http_status_message[code];
    return *this;
}

// WARN: kopirati string ili da bude const ref ili std::move()?
Response& Response::send(std::string str) {
    this->data.append(str);
    headers["Content-Type"]   = "text/html";
    headers["Content-Length"] = std::to_string(this->data.size());

    return *this;
}

std::string Response::to_string() {
    // create response str
    std::string res;
    res.reserve(this->http_version.length() +
                std::to_string(http_status_code[this->status_code]).length() +
                sizeof(http_status_message[this->status_code]) + 4 +
                this->headers.size() * 64);  // 4 for the spaces and CRLF

    res.append(this->http_version)
        .append(" ")
        .append(std::to_string(http_status_code[this->status_code]))
        .append(" ")
        .append(http_status_message[this->status_code])
        .append("\r\n");

    // append headers
    for (const auto& [key, value] : this->headers) {
        res.append(key).append(": ").append(value).append("\r\n");
    }
    res.append("\r\n");

    if (!data.empty()) {
        res.append(data);
    }

    return res;
}
