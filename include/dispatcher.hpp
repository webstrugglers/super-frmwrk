#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "constants.hpp"
#include "router.hpp"

constexpr int     REQ_BUF_SIZE = 4096;
constexpr int     MAX_RETRIES  = 2;  // Maximum number of retries to get headers
constexpr int     SLEEP_TIME_MS    = 500;  // Sleep time in milliseconds
constexpr ssize_t MAX_HEADERS_SIZE = 16384;
constexpr size_t  MAX_BODY_SIZE    = 16384;

/**
 * @brief The dispatcher function to handle incoming requests.
 *
 * This function runs in a separate thread for each connection and is
 * responsible for reading data from the socket, processing the request, and
 * sending a response.
 *
 * @param csock The socket file descriptor for the client connection.
 * @param router Reference to the router
 */
void take_over(SOCKET_FD csock, Router& router);

/**
 * @brief Read request line and headers
 *
 * @param csock client socket
 * @param buffer to which buffer to read
 * @return The starting index of the payload. Returns std::string::npos in case
 * of a bad request (e.g. bad request, doesn't respect HTTP)
 */
std::size_t recv_headers(SOCKET_FD csock, std::string& request);

void recv_body(SOCKET_FD csock, std::string& request, size_t& length);

#endif  // !DISPATCHER_HPP
