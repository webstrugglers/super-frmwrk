#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <filesystem>
#include <string>
#include <unordered_map>
#include "constants.hpp"
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
    HttpStatus  status_code;
    std::string http_version;   /**< The HTTP version (e.g., "HTTP/1.1") */
    std::string status_message; /**< The status message associated with the
                                   status code. */
    std::string data; /**< Represents the content of the HTTP response body.*/
    std::filesystem::path file_path; /**< Represents file that should be sent.*/
    std::unordered_map<std::string, std::string> headers;

public:
    Response();

    /** Sets the response’s HTTP header field to value.
     * res.set('Content-Type', 'text/plain')
     * @param field
     * @param value
     */
    Response& set(const char* field, const char* value);

    /** Sets the response’s HTTP header field to value.
     * Accepts unordered_map of headers.
     * res.set({{'header-name', 'value'}, {'header-name', 'value'}})
     * @param field
     * @param value
     */
    Response& set(const super::Headers& _headers);

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
     * It sets Content-Type header to text/plain if DATA IS NOT present in
     * response body. If DATA IS present in response body this function will
     * just update the data.
     */
    Response& send(std::string str);

    /** Sets the body for the response.
     * When serializing response object, it will automatically assign the
     * Content-Length and Content-Type headers.
     */
    Response& json(std::string str);

    /**
     * Sets the response to serve a file as an attachment.
     *
     * This function assigns the specified file path to the response,
     * indicating that the file should be sent as part of the HTTP response.
     * The file size will be determined during the serialization process.
     *
     * @param path The file path to be attached to the response.
     * @return A reference to the current `Response` object for method chaining.
     */
    Response& attachment(const std::filesystem::path& path);

    /**
     * Converts Response object to string (http representation), which will be
     * used by dispatcher. Generates status line, headers and optional body
     * @brief Returns string (http) representation of Response object
     *
     * @return String (http) representation of Response object
     */
    std::string to_string() const;

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
