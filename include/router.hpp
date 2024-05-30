#include <functional>
#include <unordered_map>
#include "path-and-type.hpp"

class Router {
     private:
    std::unordered_map<PathAndType, std::function<void()>> routing_table;
};
