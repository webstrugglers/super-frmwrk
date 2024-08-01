#include <iostream>
#include "constants.hpp"
#include "request.hpp"
#include "router.hpp"
#include "server.hpp"

void hello_world(const Request& req, Response& res) {
    res.send("Hello, world from controller function!!").status(OK);
}

void init_router(Router& router) {
    router.route(HTTP_GET, "/home", hello_world);
    router.serve_static("/home/mrda/Work/biskainc/dist/");
}

int main() {
    std::uint16_t port;
    std::cin >> port;

    Router router;
    init_router(std::ref(router));

    Server server;
    server.start(port, router);

    return 0;
}
