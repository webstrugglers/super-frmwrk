#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "constants.hpp"

/**
 * @brief The dispatcher function to handle incoming requests.
 *
 * This function runs in a separate thread for each connection and is
 * responsible for reading data from the socket, processing the request, and
 * sending a response.
 *
 * @param client_socket The socket file descriptor for the client connection.
 */
void take_over(SOCKET_FD socket);

#endif  // !DISPATCHER_HPP
