#include <iostream>
#include "constants.hpp"
#include "request.hpp"
#include "router.hpp"
#include "server.hpp"

/*#define port 5000*/

void hello_world(const Request& /*unused*/, Response& res) {
    res.body("Hello, world from controller function!!").status(OK);
}

void init_router(Router& router) {
    router.route(HTTP_GET, "/home", hello_world);
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
