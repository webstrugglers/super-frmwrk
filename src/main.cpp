#include "server.hpp"

#define port 5000

void hello_world(const Request& req, Response& res) {
    res.send("Hello, world from controller function!!").status(OK);
}

void init_router(Router& router) {
    router.route(HTTP_GET, "/controller", hello_world);
    router.serve_static("./public/");
    router.not_found("./public/notfound.html");
}

int main() {
    Router router;
    init_router(router);

    Server server;
    server.start(port, router);

    return 0;
}
