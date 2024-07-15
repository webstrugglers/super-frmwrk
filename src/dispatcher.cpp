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

// TODO:
void take_over(SOCKET_FD csock, Router& router) {
    std::cout << "Hello, world from dispatcher\n";

    ssize_t     bytes_received = 0;
    std::string request;
    ReqParser   rqp;

    // receive raw http request
    std::array<char, REQ_BUF_SIZE> buffer{};
    bytes_received = recv(csock, buffer.data(), buffer.size(), 0);
    if (bytes_received < 0) {
        perror("recv failed");
    }

    request.append(buffer.data(), bytes_received);

    std::cout << request;

    auto headers_end = request.find("\r\n\r\n");

    if (headers_end == std::string::npos) {
        perror("Error parsing request");
        close(csock);
        return;
    }

    auto sss = request.substr(0, headers_end);
    rqp.parseHeaderSection(sss);
    std::unique_ptr<Request> req = rqp.moveRequest();

    Response res;

    auto fja = router.table()[PathAndType("/home", HTTP_GET)];

    if (fja != nullptr) {
        std::cout << "Pronasao fja iterator";
    } else {
        close(csock);
        return;
    }

    fja(*req, std::ref(res));

    const auto* body = res.pSendData();
    if (sizeof(body) != 0) {
        std::cout << "Body je veci od 0";
    }

    // Prepare the HTTP response header
    std::string response_header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " +
        std::to_string(strlen(body)) +
        "\r\n"
        "\r\n";
    response_header.append(body);

    send(csock, response_header.data(), response_header.size(), 0);

    close(csock);
}
