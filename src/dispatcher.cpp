/**
 * @file dispatcher.cpp
 * @brief This file implements the dispatcher thread
 *
 * Server spawns new thread for each connection and this thread is
 * responsible for reading data from the socket, processing the request, and
 * sending a response.
 */

#include "dispatcher.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "constants.hpp"
#include "req-parser.hpp"

std::size_t recv_headers(SOCKET_FD csock, std::string& request) {
    ssize_t     bytes_received       = 0;
    ssize_t     total_bytes_received = 0;
    std::size_t headers_end          = std::string::npos;
    std::string buffer;
    buffer.resize(REQ_BUF_SIZE);

    for (int i = 0; i <= MAX_RETRIES; ++i) {
        // WARN: ovo moze zauvek da blokira
        bytes_received = recv(csock, buffer.data(), buffer.size(), 0);

        if (bytes_received > 0) {
            total_bytes_received += bytes_received;
            request.append(buffer.data(), bytes_received);
            headers_end = request.find("\r\n\r\n");
            if (headers_end != std::string::npos) {
                break;  // headers found
            }
            if (total_bytes_received > MAX_HEADERS_SIZE) {
                break;  // headers too big
            }

            // reset counter because data could come in bunch of chunks due to
            // latency
            --i;
        } else {
            break;
        }
    }

    return headers_end;
}

void take_over(SOCKET_FD csock, Router& router) {
    std::cout << "Hello, world from dispatcher\n";

    std::unique_ptr<Request> req;
    Response                 res;

    {
        std::string request;
        ReqParser   rqp;

        auto headers_end = recv_headers(csock, request);
        if (headers_end == std::string::npos) {
            perror("recv failed");
            close(csock);
            return;
        }

        std::cout << request;

        req = rqp.parseHeaderSection(request.substr(0, headers_end));
    }

    router.call(req->getPathAndType(), *req, res);

    auto odg = res.to_string();

    // TODO: handle error
    send(csock, odg.data(), odg.size(), 0);
    close(csock);
}
