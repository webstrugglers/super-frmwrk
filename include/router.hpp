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

    /**
     * @brief Path to the directory where we look for static files
     *
     * This path is set by Router::serve_static method
     */
    std::filesystem::path static_root;

    std::filesystem::path not_found_page;

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
    void call(const Request& req, Response& res);

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
     * @brief Configures the router to serve static files from a specified
     * directory.
     *
     * This method sets up the router to serve static files located in the
     * given directory and its subdirectories. It creates routes for each file,
     * allowing them to be accessed via HTTP GET requests.
     * Calling this method multiple times will result in undefined behaviour.
     *
     * @param path The directory containing the static files to be served.
     */
    void serve_static(const std::filesystem::path& path);

    /**
     * @brief Set your custom 404 page
     *
     * @param path Path to your "page not found" html document
     */
    void not_found(const std::filesystem::path& path);

private:
    void handle_route(std::function<void(const Request&, Response&)>& handler,
                      const Request&                                  req,
                      Response&                                       res);

    void set_static_root(const std::filesystem::path& path);

    /**
     * @brief Sets not found status for response. If html document was
     * requested, additionally returns page not found document
     *
     */
    void res_not_found(const PathAndType& pat, Response& res);

    /**
     * @brief Attempts to locate the requested file if no handler is available.
     *
     */
    void potential_static(const Request& req, Response& res);

    /**
     * @brief Verifies that the provided file path is both valid and points to
     * an existing file.
     */
    bool is_req_file_legit(const std::filesystem::path& p);

    /**
     * @brief if path/index.html exists map GET / to it
     */
    void map_root_to_index();

    /**
     * @brief set date header
     *
     */
    void set_date_header(Response& res);
};

#endif  // !ROUTER_HPP
