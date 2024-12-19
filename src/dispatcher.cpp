/**
 * @file dispatcher.cpp
 * @brief This file implements the dispatcher thread
 *
 * Server spawns new thread for each connection and this thread is
 * responsible for reading data from the socket, processing the request, and
 * sending a response.
 */

#include "dispatcher.hpp"
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <memory>
#include <string>
#include "constants.hpp"
#include "req-parser.hpp"

/**
 * @brief Read request line and headers
 *
 * @param csock client socket
 * @param buffer to which buffer to read
 * @return The starting index of the payload. Returns std::string::npos in case
 * of a bad request (e.g. bad request, doesn't respect HTTP)
 */
namespace {
std::size_t recv_headers(SOCKET_FD csock, std::string& request) {
    ssize_t     bytes_received       = 0;
    ssize_t     total_bytes_received = 0;
    std::size_t headers_end          = std::string::npos;
    std::string buffer;
    buffer.resize(REQ_BUF_SIZE);

    for (int i = 0; i <= MAX_RETRIES; ++i) {
        bytes_received = recv(csock, buffer.data(), buffer.size(), 0);

        if (bytes_received > 0) {
            total_bytes_received += bytes_received;
            request.append(buffer.data(), bytes_received);
            headers_end = request.find("\r\n\r\n");
            if (headers_end != std::string::npos) {
                break;  // headers found
            }
            if (total_bytes_received >= MAX_HEADERS_SIZE) {
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

void recv_body(int csock, std::string& request, ssize_t length) {
    ssize_t     bytes_received       = 0;
    ssize_t     total_bytes_received = 0;
    std::string buffer;
    auto        start_length = static_cast<long>(request.length());
    buffer.resize(REQ_BUF_SIZE);

    for (int i = 0; i <= MAX_RETRIES; ++i) {
        bytes_received = recv(csock, buffer.data(), buffer.size(), 0);
        if (bytes_received > 0) {
            total_bytes_received += bytes_received;
            request.append(buffer.data(), bytes_received);

            if (total_bytes_received > MAX_BODY_SIZE) {
                break;  // body size too big
            }

            if (bytes_received > length) {
                break;
            }
            if (total_bytes_received == length - start_length) {
                break;  // whole body read
            }
            --i;
        } else {
            break;
        }
    }
}
}  // namespace

void take_over(SOCKET_FD csock, Router& router) {
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

        ssize_t length  = 0;
        req             = rqp.parseRequest(request);
        auto len_header = req->headers.find("Content-Length");
        if (len_header != req->headers.end()) {
            req->body = "";
            try {
                length = std::stol(len_header->second);
            } catch (std::exception& e) {
                close(csock);
                return;
            }
            if (length < 0 || length > MAX_BODY_SIZE) {
                close(csock);
                return;
            }

            if (static_cast<long>(request.length()) != length) {
                recv_body(csock, request, length);
            }

            req->body = request;
        }
    }

    router.call(*req, res);
    auto odg = res.to_string();

    ssize_t sent_bytes = send(csock, odg.data(), odg.size(), 0);
    if (sent_bytes == -1) {
        perror("send");
        close(csock);
        return;
    }

    auto file = res.file();
    if (std::filesystem::exists(file) && !file.empty()) {
        int fd = open(file.c_str(), O_RDONLY);
        if (fd == -1) {
            perror("open");
            close(csock);
            return;
        }

        off_t   offset = 0;
        ssize_t sent_file_bytes =
            sendfile(csock, fd, &offset, std::filesystem::file_size(file));
        if (sent_file_bytes == -1) {
            // Handle sendfile error
            perror("sendfile");
        }
    }

    if (close(csock) == -1) {
        // Handle close error
        perror("close");
    }
}
