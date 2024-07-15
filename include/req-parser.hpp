#ifndef REQPARSER_HPP
#define REQPARSER_HPP

#include <memory>
#include "request.hpp"

class ReqParser {
private:
    std::unique_ptr<Request> req;

public:
    ReqParser();

    void                     parseReqLine(std::string str);
    void                     parseHeaders(std::string str);
    void                     parseHeaderSection(const std::string& str);
    std::unique_ptr<Request> moveRequest();
};

#endif  // DEBUG
