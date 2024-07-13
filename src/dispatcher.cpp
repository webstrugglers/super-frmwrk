/**
 * @file dispatcher.cpp
 * @brief This file implements the dispatcher thread
 *
 * This function runs in a separate thread for each connection and is
 * responsible for reading data from the socket, processing the request, and
 * sending a response.
 */

#include "dispatcher.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "req-parser.hpp"

// TODO:
void take_over(SOCKET_FD socket) {
    std::cout << "Hello, world from dispatcher\n";

    char        buffer[REQ_BUF_SIZE];
    ssize_t     bytes_received;
    std::string request;

    // FIX: 1. optimizovati
    //      2. ne prekida konekciju
    while ((bytes_received = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        request.append(buffer, bytes_received);

        memset(buffer, 0, sizeof(buffer));
    }

    // Check for errors or end-of-file
    if (bytes_received < 0) {
        perror("recv failed");
    }

    close(socket);

    std::cout << request;

    RequestParser requestParser(request);
    Request       newRequest = requestParser.parseRequest(request);
    std::cout << "\nMethod: " << newRequest.getMethodType() << std::endl;
    std::cout << "Path: " << newRequest.getPath() << std::endl;
    std::cout << "Headers:" << std::endl;
    for (const auto& header : newRequest.getHeaders()) {
        std::cout << header.first << ":" << header.second << std::endl;
    }
    std::cout << "Query params:" << std::endl;
    for (const auto& param : newRequest.getQueryParams()) {
        std::cout << param.first << ":" << param.second << std::endl;
    }
    std::cout << "Body:" << std::endl << newRequest.getBody() << std::endl;
}
