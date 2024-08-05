#include <iostream>
#include "constants.hpp"
#include "request.hpp"
#include "router.hpp"
#include "server.hpp"

void hello_world(const Request& req, Response& res) {
    res.send("Hello, world from controller function!!").status(OK);
}

void init_router(Router& router) {
    router.route(HTTP_GET, "/controller", hello_world);
    router.serve_static("./public/");
    router.not_found("./public/notfound.html");
}

int main() {
    std::uint16_t port = 0;
    std::cin >> port;

    Router router;
    init_router(std::ref(router));

    Server server;
    server.start(port, router);

    return 0;
}
