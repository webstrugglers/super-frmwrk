#include "server.hpp"
#include <netinet/in.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <thread>
#include "dispatcher.hpp"

Server::Server() {}

Server::Server(int port) {
    start(port);
}

void Server::start(std::uint16_t port) {
    uint16_t  network_port = htons(port);
    SOCKET_FD s;

    struct sockaddr_in address;
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = network_port;

    // create socket fd
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        std::cerr << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // bind port and addr
    if (bind(s, (struct sockaddr*)&address, sizeof(address)) == -1) {
        std::cerr << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // start listening
    if (listen(s, 10) == -1) {
        std::cerr << strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // server loop
    // let dispatcher take over request
    while (true) {
        SOCKET_FD new_socket = accept(s, 0, 0);
        if (new_socket == -1) {
            std::cerr << strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::thread dispatcher = std::thread(take_over, new_socket);

        dispatcher.detach();
    }
}
