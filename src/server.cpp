/**
 * @file
 * @brief Implements HTTP server
 */

#include "server.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include "dispatcher.hpp"
#include "logger.hpp"

Server::Server() : ss(-1) {}

Server::~Server() {
    if (this->ss != -1) {
        close(this->ss);
    }
}

void Server::start(std::uint16_t port) {
    uint16_t  network_port  = htons(port);
    SOCKET_FD server_socket = -1;

    struct sockaddr_in address {};
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = network_port;

    // create socket fd
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        SafeLogger::log(errno);
        return;
    }

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) ==
        -1) {
        SafeLogger::log(errno);
        return;
    }

    // start listening
    if (listen(server_socket, 10) == -1) {
        SafeLogger::log(errno);
        return;
    }

    ;
    SafeLogger::log("Server listening on port " + std::to_string(port));

    // server loop
    // let dispatcher take over request
    while (true) {
        SOCKET_FD new_socket = accept(server_socket, nullptr, nullptr);
        if (new_socket == -1) {
            SafeLogger::log(errno);
            return;
        }

        std::thread dispatcher = std::thread(take_over, new_socket);

        dispatcher.detach();
    }
}
