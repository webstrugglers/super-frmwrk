#include "dispatcher.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include "constants.hpp"

// TODO:
void take_over(SOCKET_FD socket) {
    std::cout << "Hello, world from dispatcher\n";

    char        buffer[REQ_BUF_SIZE];
    ssize_t     bytes_received;
    std::string request;

    // FIX:
    while ((bytes_received = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        request.append(buffer, bytes_received);

        memset(buffer, 0, sizeof(buffer));
    }

    // Check for errors or end-of-file
    if (bytes_received < 0)
        perror("recv failed");


    close(socket);
}
