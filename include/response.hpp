#include <filesystem>
#include <string>
#include <unordered_map>
#include "constants.hpp"

class Response {
private:
    std::string data; /**< Represents the content of the HTTP response body.*/
    std::string http_version; /**< The HTTP version (e.g., "HTTP/1.1") */
    std::string
        reason_phrase; /**< The reason phrase associated with the status code */
    HttpStatus                                   status_code;
    std::unordered_map<std::string, std::string> headers;

    std::filesystem::path
        file_name; /**< TODO: zahteva poseban struggle jer verovatno treba
                        koristiti sendfile syscall*/

public:
    Response();

    Response& status(const HttpStatus status_code);
    Response& body(const char* body_str);
};
