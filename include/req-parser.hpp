#ifndef REQPARSER_HPP
#define REQPARSER_HPP

#include <memory>
#include "request.hpp"

/**
 * @class ReqParser
 * @brief Used for transforming http request to Request object
 *
 */
class ReqParser {
private:
    std::unique_ptr<Request> req;

public:
    ReqParser();

    void                     parseReqLine(std::string str);
    void                     parseHeaders(std::string str);
    std::unique_ptr<Request> parseHeaderSection(const std::string& str);
};

#endif  // DEBUG
