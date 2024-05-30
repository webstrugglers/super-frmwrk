#include <iostream>
#include "path-and-type.hpp"

void mojafunc1() {
    std::cout << "Hello pat 1\n";
}

void mojafunc2() {
    std::cout << "Hello pat 2\n";
}

int main() {
    PathAndType pat;
    PathAndType pat2("/home", POST);

    std::unordered_map<PathAndType, std::function<void()>> mapa = {
        {pat, mojafunc1},
        {pat2, mojafunc2},
    };

    mapa[pat2]();

    return 0;
}
