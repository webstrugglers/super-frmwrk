#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include "constants.hpp"

// http://expressjs.com/en/5x/api.html#res
// po uzoru na express
class Response {
private:
    std::string http_version; /**< The HTTP version (e.g., "HTTP/1.1") */
    HttpStatus  status_code;
    std::string status_message; /**< The status message associated with the
                                   status code. */
    std::unordered_map<std::string, std::string> headers;
    std::string data; /**< Represents the content of the HTTP response body.*/

public:
    Response();

    /** Sets the response’s HTTP header field to value.
     * res.set('Content-Type', 'text/plain')
     * @param field
     * @param value
     */
    Response& set(char* field, char& value);
    /**
     * Sets the HTTP status for the response.
     * It will also automatically set the status message.
     */
    Response& status(const HttpStatus status_code);
    /** Sets the body for the response.
     * When serializing response object, it will automatically assign the
     * Content-Length HTTP response header field.
     */
    Response& body(std::string str);

    const char* pSendData() const;

private:
    // TODO: lista todo stvari za ovaj objekat

    // najverovatnije treba koristiti sendfile syscall
    Response&             attachment(std::filesystem::path file_name);
    std::filesystem::path file_name;

    // dinamicko prevodjenje podataka u json. Nama nepoznat objekat
    Response& json();
};

#endif  // !RESPONSE_HPP
