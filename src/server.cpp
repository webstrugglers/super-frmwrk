/**
 * @file
 * @brief Implements HTTP server
 */

#include "server.hpp"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <functional>
#include <thread>
#include "dispatcher.hpp"
#include "logger.hpp"

Server* Server::me = nullptr;

Server::Server() : ss(-1), stop_flag(false) {
    Server::me = this;
}

Server::~Server() {
    if (this->ss != -1) {
        close(this->ss);
    }
}

void Server::start(std::uint16_t port, Router& router) {
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
    this->ss = server_socket;

    int optval = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval,
                   sizeof(optval)) < 0) {
        SafeLogger::log(errno);
        close(server_socket);
        return;
    }

    struct sigaction a {};
    a.sa_handler = Server::signal_handler;
    a.sa_flags   = 0;
    sigemptyset(&a.sa_mask);
    sigaction(SIGINT, &a, NULL);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) ==
        -1) {
        SafeLogger::log(errno);
        close(server_socket);
        return;
    }

    // start listening
    if (listen(server_socket, 10) == -1) {
        SafeLogger::log(errno);
        close(server_socket);
        return;
    }

    SafeLogger::log("Server listening on port " + std::to_string(port));

    struct sockaddr_in client_addr {};
    socklen_t          client_len = sizeof(client_addr);

    // server loop
    // let dispatcher take over request
    while (!this->stop_flag.load()) {
        // accept connection
        SOCKET_FD new_socket =
            accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (new_socket == -1) {
            SafeLogger::log(errno);
            continue;
        }

        std::string client_ip   = inet_ntoa(client_addr.sin_addr);
        std::string client_port = std::to_string(ntohs(client_addr.sin_port));
        SafeLogger::log("Connection accepted from " + client_ip + ":" +
                        client_port);

        std::thread dispatcher =
            std::thread(take_over, new_socket, std::ref(router));

        dispatcher.detach();
    }
    close(this->ss);
    this->ss = -1;
}

void Server::handle_signal() {
    this->stop_flag.store(true);
}
