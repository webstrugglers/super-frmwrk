#include "dispatcher.hpp"
#include <sys/socket.h>
#include <cstdio>
#include <iostream>
#include <string>

void take_over(SOCKET_FD socket) {
    std::cout << "Hello, world from dispatcher\n";
    char        buffer[4096] = {0};
    std::string request;
    while (recv(socket, buffer, 4096, 0) > 0) {
        request.append(buffer);
    }

    perror("hello");
}
