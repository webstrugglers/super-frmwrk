#include <functional>
#include <unordered_map>
#include "path-and-type.hpp"
#include "request.hpp"
#include "response.hpp"

class Router {
private:
    std::unordered_map<
        PathAndType,
        std::function<void(const Request& req, const Response& res)>>
        routing_table;

public:
    Router();

    /**
     * @brief Returns routing table used for routing
     *
     */
    std::unordered_map<
        PathAndType,
        std::function<void(const Request& req, const Response& res)>>
    table() const;

    /**
     * @brief Maps an HTTP method and path to a controller function.
     *
     * This function associates an HTTP method and a specific path with a
     * controller function that will handle requests to that path.
     *
     * @param method_type The HTTP method (GET, POST, etc.).
     * @param path The URL path.
     * @param controller The controller function to handle the request.
     */
    void route(const MethodType                                method_type,
               const char*                                     path,
               const std::function<void(const Request&  req,
                                        const Response& res)>& controller);

    void get(const char* path,
             const std::function<void(const Request& req, const Response& res)>&
                 controller);
    void put(const char* path,
             const std::function<void(const Request& req, const Response& res)>&
                 controller);
    void post(const char*                                     path,
              const std::function<void(const Request&  req,
                                       const Response& res)>& controller);
};
