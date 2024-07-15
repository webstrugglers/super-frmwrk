#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <functional>
#include "request.hpp"
#include "response.hpp"

// TODO: jos metoda za lakse koriscenje
/**
 * @class Router
 */
class Router {
private:
    std::unordered_map<PathAndType,
                       std::function<void(const Request& req, Response& res)>>
        routing_table; /**< Internal map that associates HTTP methods and paths
                          with controller functions. */

public:
    Router();

    /**
     * @brief Returns routing table used for routing
     *
     */
    std::unordered_map<PathAndType,
                       std::function<void(const Request& req, Response& res)>>&
    table();

    /**
     * @brief Maps an HTTP method and path to a controller function.
     *
     * Associates a specified HTTP method and URL path with a controller
     * function that will handle incoming requests matching the given method and
     * path.
     *
     * @param method_type The HTTP method (e.g., GET, POST).
     * @param path The URL path.
     * @param controller The controller function to handle the requests.
     */
    void route(const MethodType method_type,
               const char*      path,
               const std::function<void(const Request& req, Response& res)>&
                   controller);

    /**
     * @brief Maps a PathAndType object to a controller function.
     *
     * Associates a PathAndType object, which contains an HTTP method and URL
     * path, with a controller function that will handle incoming requests
     * matching the given method and path.
     *
     * @param pat The PathAndType object containing the HTTP method and URL
     * path.
     * @param controller The controller function to handle the requests.
     */
    void route(PathAndType& pat,
               const std::function<void(const Request& req, Response& res)>&
                   controller);

    void get(const char* path,
             const std::function<void(const Request& req, Response& res)>&
                 controller);
    void put(const char* path,
             const std::function<void(const Request& req, Response& res)>&
                 controller);
    /**
     * @brief workflow test 5
     *
     */
    void post(const char* path,
              const std::function<void(const Request& req, Response& res)>&
                  controller);
};

#endif  // !ROUTER_HPP
