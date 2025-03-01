/**
 * @file
 * @brief Implements the Response class used for representing HTTP requests
 */

#include "response.hpp"
#include <string>
#include "http-status-codes.hpp"

Response::Response()
    : status_code(NOT_FOUND),
      http_version("HTTP/1.0"),
      status_message(http_status_message[NOT_FOUND]) {}

Response& Response::set(const char* field, const char* value) {
    this->headers.emplace(field, value);

    return *this;
}

Response& Response::set(const std::basic_string<char>& field,
                        const std::basic_string<char>& value) {
    this->headers.emplace(field, value);
    return *this;
}

Response& Response::set(const super::Headers& _headers) {
    for (const auto& pair : _headers) {
        this->headers.emplace(pair);
    }

    return *this;
}

Response& Response::status(const HttpStatus code) {
    this->status_code    = code;
    this->status_message = http_status_message[code];
    return *this;
}

Response& Response::send(std::string str) {
    this->data = std::move(str);

    // don't update Content-Type header if it already exists
    // this is usefull if we want to alter data
    headers.emplace("Content-Type", "text/plain");
    headers["Content-Length"] = std::to_string(this->data.size());

    return *this;
}

Response& Response::json(std::string str) {
    this->data                = std::move(str);
    headers["Content-Type"]   = "application/json";
    headers["Content-Length"] = std::to_string(this->data.size());

    return *this;
}

Response& Response::attachment(const std::filesystem::path& path) {
    // file size will be calculated when serializing
    this->file_path = path;
    return *this;
}

std::string Response::to_string() const {
    // create response str
    std::string res;
    res.reserve(this->http_version.length() +
                std::to_string(http_status_code[this->status_code]).length() +
                sizeof(http_status_message[this->status_code]) + 4 +
                (this->headers.size() * 64));  // 4 for the spaces and CRLF

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

std::filesystem::path Response::file() const {
    return this->file_path;
}

const std::string& Response::get_data() const noexcept {
    return this->data;
}
