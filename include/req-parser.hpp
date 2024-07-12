#ifndef REQ_PARSER_HPP
#define REQ_PARSER_HPP
#include <string>
#include "request.hpp"
class RequestParser {
private:
    std::string rawHttpRequest;

public:
    explicit RequestParser(const std::string& request);

    static void printRawHttpRequest(const std::string& request);

    Request parseRequest(const std::string& request);
};
#endif
