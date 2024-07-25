#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <filesystem>
#include <functional>
#include <memory>
#include "request.hpp"
#include "response.hpp"

// TODO: jos metoda za lakse koriscenje
/**
 * @class Router
 * @brief Class used for routing
 *
 * Router uses std::unordered_map internally to map HTTP request method and path
 * to a developer defined controller function.
 */
class Router {
private:
    std::unique_ptr<std::unordered_map<
        PathAndType,
        std::function<void(const Request& req, Response& res)>>>
        routing_table; /**< Internal hashmap that associates HTTP methods and
                          paths with controller functions. */

    /**
     * Hashmap that maps file extensions to their respective MIME types. Only
     * contains common MIME types
     * (defined by Mozilla
     * developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types)
     */
    std::unique_ptr<std::unordered_map<std::string, std::string>> mimes;

public:
    Router();

    /**
     * @brief Used internally to allow dispatcher to call appropriate
     * controller.
     *
     * A method that will call the appropriate controller function based on the
     * provided PathAndType object. Used internally to allow dispatcher to call
     * appropriate controller.
     *
     * @param pat PathAndType object used for finding appropriate controller
     * @param req Reference to Request object representing HTTP request
     * @param res Reference to Response object that is manipulated by developer
     */
    void call(const PathAndType& pat, const Request& req, Response& res);

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
     * @brief Same as Router::route(HTTP_GET, path, controller)
     *
     * @param path The URL path.
     * @param controller The controller function to handle the requests.
     */
    void get(const char* path,
             const std::function<void(const Request& req, Response& res)>&
                 controller);

    /**
     * @brief Same as Router::route(HTTP_PUT, path, controller)
     *
     * @param path The URL path.
     * @param controller The controller function to handle the requests.
     */
    void put(const char* path,
             const std::function<void(const Request& req, Response& res)>&
                 controller);
    /**
     * @brief Same as Router::route(HTTP_POST, path, controller)
     *
     * @param path The URL path.
     * @param controller The controller function to handle the requests.
     */
    void post(const char* path,
              const std::function<void(const Request& req, Response& res)>&
                  controller);

    /**
     * Maps HTTP GET requests + path to files in the specified directory
     *
     * This method configures the router to serve static files located in the
     * given directory. The path of the requested file is mapped relative to the
     * specified directory.
     *
     * @param path The path to the directory containing static files.
     */
    void serve_static(const std::filesystem::path& path);
};

#endif  // !ROUTER_HPP
