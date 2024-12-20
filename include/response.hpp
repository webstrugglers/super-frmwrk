#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include "http-status-codes.hpp"

/**
 * @class Response
 * @brief A class to represent HTTP response. For developers to use.
 *
 * This class will be transformed to HTTP response and sent to the client.
 * Provides expressjs-like API for developer to manipulate and use for crafting
 * responses(e.g. res.status(HttpStatus).send("Hello")).
 */
class Response {
private:
    std::string http_version; /**< The HTTP version (e.g., "HTTP/1.1") */
    HttpStatus  status_code;
    std::string status_message; /**< The status message associated with the
                                   status code. */
    std::unordered_map<std::string, std::string> headers;
    std::string data; /**< Represents the content of the HTTP response body.*/
    std::filesystem::path file_path; /**< Represents file that should be sent.*/

public:
    Response();

    /** Sets the response’s HTTP header field to value.
     * res.set('Content-Type', 'text/plain')
     * @param field
     * @param value
     */
    Response& set(const char* field, const char* value);

    /** Sets the response’s HTTP header field to value.
     * res.set('Content-Type', 'text/plain')
     * @param field Header name
     * @param value Header value
     */
    Response& set(const std::basic_string<char>& field,
                  const std::basic_string<char>& value);

    /**
     * Sets the HTTP status code and message for the response.
     */
    Response& status(HttpStatus status_code);

    /** Sets the body for the response.
     * When serializing response object, it will automatically assign the
     * Content-Length header.
     */
    Response& send(std::string str);

    /** Sets the body for the response.
     * When serializing response object, it will automatically assign the
     * Content-Length and Content-Type headers.
     */
    Response& json(std::string str);

    /**
     * @brief [TODO:description]
     *
     * @param path [TODO:parameter]
     * @return [TODO:return]
     */
    Response& attachment(const std::filesystem::path& path);

    /**
     * Converts Response object to string (http representation), which will be
     * used by dispatcher. Generates status line, headers and optional body
     * @brief Returns string (http) representation of Response object
     *
     * @return String (http) representation of Response object
     */
    std::string to_string();

    /**
     * @brief Used internally.
     *
     * Checks if the response should return file, so we can determine which
     * syscall to call.
     *
     * @return file path to the file
     */
    std::filesystem::path file() const;

    /**
     * Returns const reference to the reponse body data.
     */
    const std::string& get_data() const noexcept;

private:
    // TODO: lista todo stvari za ovaj objekat
};

#endif  // !RESPONSE_HPP
