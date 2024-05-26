#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdint>

class Server {
     private:
     public:
    Server();
    Server(int port);

    // TODO:
    ~Server();

    void start(std::uint16_t port);

     private:
};

#endif  // !SERVER_HPP
