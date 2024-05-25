#include "dispatcher.hpp"
#include <sys/socket.h>
#include <iostream>

bool take_over(SOCKET_FD socket) {
    std::cout << "Hello, world from dispatcher\n";
    return false;
}
