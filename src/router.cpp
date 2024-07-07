/**
 * @file
 * @brief Implements Router class for mapping http request to specified
 * controllers
 */

#include "router.hpp"
#include <utility>
#include "constants.hpp"
#include "logger.hpp"
#include "path-and-type.hpp"

Router::Router() = default;

// TODO: dodati provere da li putanje vec postoje!

void Router::route(
    const MethodType                                              method_type,
    const char*                                                   path,
    const std::function<void(const Request& req, Response& res)>& controller) {
    PathAndType pat = PathAndType(path, method_type);

    if (routing_table.find(pat) != routing_table.end()) {
        SafeLogger::log("You tried mapping Path (" + pat.getPath() +
                        ") and method type (" + pat.getMethodType() +
                        ") more than once");
        exit(EXIT_FAILURE);
    }

    routing_table.insert(std::pair(pat, controller));
}

void Router::route(
    PathAndType&                                                  pat,
    const std::function<void(const Request& req, Response& res)>& controller) {
    if (routing_table.find(pat) != routing_table.end()) {
        SafeLogger::log("You tried mapping Path (" + pat.getPath() +
                        ") and method type (" + pat.getMethodType() +
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
                   std::function<void(const Request& req, Response& res)>>
Router::table() const {
    return routing_table;
}
