#include <iostream>
#include "constants.hpp"
#include "path-and-type.hpp"
#include "request.hpp"
#include "router.hpp"
#include "server.hpp"

void mojafunc1() {
    std::cout << "Hello pat 1\n";
}

void mojafunc2() {
    std::cout << "Hello pat 2\n";
}

void mojafunc3(const Request& req, const Response& res) {
    std::cout << "Hello pat 3\n";
}

void controller(const Request& req, Response& res) {
    res.status(OK);
    std::cout << "Hello from controller\n";
}

int main() {
    PathAndType pat;
    PathAndType pat2("/home", HTTP_POST);
    PathAndType pat3("/home", HTTP_PUT);
    PathAndType pat4("/home", HTTP_POST);

    /*std::unordered_map<PathAndType, std::function<void()>> mapa = {*/
    /*    {pat, mojafunc1},*/
    /*};*/

    /*mapa[pat2]();*/

    Router   router;
    Request  req;
    Response res;

    router.route(pat4, controller);

    Server server;
    server.start(5000);

    return 0;
}
