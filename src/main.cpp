#include <iostream>
#include "constants.hpp"
#include "path-and-type.hpp"
#include "request.hpp"
#include "router.hpp"

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
    PathAndType pat2("/home", POST);
    PathAndType pat3("/home", PUT);

    std::unordered_map<PathAndType, std::function<void()>> mapa = {
        {pat, mojafunc1},
        {pat2, mojafunc2},
    };

    mapa[pat2]();

    Router router;

    router.get("/home", mojafunc3);

    PathAndType pat1("/home", GET);

    Request  req;
    Response res;

    router.table()[pat1](req, res);
    router.route(pat3, controller);

    router.table()[pat3](req, res);

    return 0;
}
