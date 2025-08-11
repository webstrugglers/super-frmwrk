/**
 * @file
 * @brief Implements HTTP server
 */

#include "server.hpp"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <functional>
#include <thread>
#include "dispatcher.hpp"
#include "logger.hpp"

Server::~Server() {
    if (this->ss != -1) {
        close(this->ss);
    }
}

void Server::configure_server_socket() const noexcept {
    int optval = 1;
    if (setsockopt(this->ss, SOL_SOCKET, SO_REUSEADDR, &optval,
                   sizeof(optval)) < 0) {
        SafeLogger::log(errno);
    }

    int optval_tcp_nodelay = 1;
    if (setsockopt(this->ss, IPPROTO_TCP, TCP_NODELAY,
                   reinterpret_cast<char*>(&optval_tcp_nodelay),
                   sizeof(int)) < 0) {
        SafeLogger::log(errno);
    }
}

void Server::start(std::uint16_t port, Router& router) {
    struct sockaddr_in address{};
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(port);

    // create socket fd
    this->ss = socket(AF_INET, SOCK_STREAM, 0);
    if (this->ss < 0) {
        SafeLogger::log(errno);
        return;
    }

    // configure socket
    configure_server_socket();

    // bind socket to port
    if (bind(this->ss, reinterpret_cast<struct sockaddr*>(&address),
             sizeof(address)) == -1) {
        SafeLogger::log(errno);
        close(this->ss);
        return;
    }

    // start listening
    if (listen(this->ss, 10) == -1) {
        SafeLogger::log(errno);
        close(this->ss);
        return;
    }

    SafeLogger::log("Server listening on port " + std::to_string(port));

    struct sockaddr_in client_addr{};
    socklen_t          client_len = sizeof(client_addr);

    // server loop
    while (true) {
        // accept connection
        SOCKET_FD client_sock =
            accept(this->ss, reinterpret_cast<struct sockaddr*>(&client_addr),
                   &client_len);
        if (client_sock == -1) {
            SafeLogger::log(errno);
            continue;
        }

        struct timeval timeout{};
        timeout.tv_sec  = 3;
        timeout.tv_usec = 0;

        if (setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                       sizeof timeout) < 0) {
            SafeLogger::log(errno);
            continue;
        }

        // let dispatcher take over request
        std::thread dispatcher =
            std::thread(take_over, client_sock, std::ref(router));

        dispatcher.detach();
    }
    if (this->ss != -1) {
        close(this->ss);
    }
    this->ss = -1;
}
