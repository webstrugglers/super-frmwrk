/**
 * @file
 * @brief Implements Router class for mapping http request to specified
 * controllers
 */

#include "router.hpp"
#include "constants.hpp"
#include "logger.hpp"

Router::Router() = default;

// TODO: dodati provere da li putanje vec postoje!

void Router::route(
    const MethodType                                              method_type,
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    PathAndType pat = PathAndType(path, method_type);

    if (routing_table.find(pat) != routing_table.end()) {
        SafeLogger::log("You tried mapping Path (" + pat.path +
                        ") and method type (" + pat.method_type +
                        ") more than once");
        exit(EXIT_FAILURE);
    }

    routing_table.insert(std::pair(pat, controller));
}

void Router::get(
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    route(HTTP_GET, path, controller);
}

void Router::post(
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    route(HTTP_POST, path, controller);
}

void Router::put(
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    route(HTTP_PUT, path, controller);
}

std::unordered_map<PathAndType,
                   std::function<void(const Request& req, Response& res)>>&
Router::table() {
    return routing_table;
}

// TODO:
void Router::call(const PathAndType& pat, const Request& req, Response& res) {
    auto func = this->routing_table.find(pat);
    if (func != this->routing_table.end()) {
        func->second(req, res);
    } else {
        res.send(PAGE_NOT_FOUND_HTML);
    }
}
