#include "dispatcher.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

// TODO:
void take_over(SOCKET_FD socket) {
    std::cout << "Hello, world from dispatcher\n";

    char        buffer[4096];
    ssize_t     bytes_received;
    std::string request;

    while ((bytes_received = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        request.append(buffer, bytes_received);

        memset(buffer, 0, sizeof(buffer));
    }

    // Check for errors or end-of-file
    if (bytes_received < 0)
        perror("recv failed");

    close(socket);
}
