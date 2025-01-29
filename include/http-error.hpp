#ifndef HTTP_ERROR
#define HTTP_ERROR

#include <string>
#include "constants.hpp"
#include "http-status-codes.hpp"

namespace super {

class HttpError {
private:
    HttpStatus  status;
    std::string message;  // Log message (not sent to client)
    Headers     headers;

public:
    HttpError(HttpStatus status_code) : status(status_code) {}

    HttpError(HttpStatus status_code, std::string new_message)
        : status(status_code),
          message(std::move(new_message)) {}

    [[nodiscard]] constexpr HttpStatus get_status() const noexcept {
        return status;
    }

    [[nodiscard]] const std::string& get_message() const noexcept {
        return message;
    }

    [[nodiscard]] const Headers& get_headers() const noexcept {
        return headers;
    }

    HttpError& add_header(const std::string& key,
                          const std::string& value) noexcept {
        if (key.empty() || value.empty()) {
            return *this;
        }
        headers.emplace(key, value);
        return *this;
    }

    HttpError& add_header(std::string&& key, std::string&& value) noexcept {
        if (key.empty() || value.empty()) {
            return *this;
        }
        headers.emplace(std::move(key), std::move(value));
        return *this;
    }

    HttpError& add_headers(const Headers& new_headers) noexcept {
        for (const auto& [key, value] : new_headers) {
            if (key.empty() || value.empty()) {
                continue;
            }
            headers.emplace(key, value);
        }

        return *this;
    }

    HttpError& add_headers(Headers&& new_headers) noexcept {
        // Check for self-assignment
        if (&headers == &new_headers) {
            return *this;
        }

        // Reserve space in headers to avoid reallocations
        headers.reserve(headers.size() + new_headers.size());

        // Move elements from new_headers to headers, skipping empty keys or
        // values
        for (auto& pair : new_headers) {
            if (pair.first.empty() || pair.second.empty()) {
                headers.insert(std::move(pair));
            }
        }

        // Optionally clear new_headers after moving its contents
        new_headers.clear();

        return *this;
    }
};

}  // namespace super

#endif
