#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>

/**
 * @class Server
 * @brief A class to represent the HTTP server.
 *
 * This class provides functionalities to start and manage an HTTP server.
 */
class Server {
private:
public:
    Server();
    Server(std::uint16_t port);

    // TODO:
    ~Server();

    /**
     * @brief Starts the server on the given port.
     *
     * This function initializes and starts the server, making it listen on the
     * specified port. It accepts incoming connections and spawns a new
     * dispatcher thread for each connection.
     *
     * @param port The port number on which the server will listen for incoming
     * connections.
     */
    void start(std::uint16_t port);
};

#endif  // !SERVER_HPP
