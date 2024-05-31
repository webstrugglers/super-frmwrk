#include "router.hpp"
#include "constants.hpp"
#include "path-and-type.hpp"

Router::Router() = default;

void Router::route(const MethodType                                method_type,
                   const char*                                     path,
                   const std::function<void(const Request&  req,
                                            const Response& res)>& controller) {
    PathAndType pat = PathAndType(path, method_type);
    routing_table.insert(std::pair(pat, controller));
}

void Router::get(const char*                                     path,
                 const std::function<void(const Request&  req,
                                          const Response& res)>& controller) {
    route(GET, path, controller);
}

void Router::post(const char*                                    path,
                  const std::function<void(const Request&  req,
                                           const Response& res)> controller) {
    route(POST, path, controller);
}

void Router::put(const char*                                    path,
                 const std::function<void(const Request&  req,
                                          const Response& res)> controller) {
    route(PUT, path, controller);
}

std::unordered_map<PathAndType,
                   std::function<void(const Request& req, const Response& res)>>
Router::table() const {
    return routing_table;
}
