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
#include <array>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include "constants.hpp"
#include "req-parser.hpp"
#include "request.hpp"

#define REQ_BUF_SIZE 4096

// TODO: thread safety kad se poziva funkcija
// pustiti router da poziva controller
void take_over(SOCKET_FD csock, Router& router) {
    std::cout << "Hello, world from dispatcher\n";

    ssize_t                  bytes_received = 0;
    unsigned long            headers_end    = 0;
    ReqParser                rqp;
    std::unique_ptr<Request> req;

    {
        std::string request;

        // receive raw http request
        std::array<char, REQ_BUF_SIZE> buffer{};
        bytes_received = recv(csock, buffer.data(), buffer.size(), 0);
        if (bytes_received < 0) {
            perror("recv failed");
        }

        request.append(buffer.data(), bytes_received);

        std::cout << request;

        headers_end = request.find("\r\n\r\n");

        if (headers_end == std::string::npos) {
            perror("Error parsing request");
            close(csock);
            return;
        }

        req = rqp.parseHeaderSection(request.substr(0, headers_end));
    }

    Response res;

    router.call(req->getPathAndType(), *req, std::ref(res));

    auto odg = res.to_string();

    send(csock, odg.data(), odg.size(), 0);
    close(csock);
}
