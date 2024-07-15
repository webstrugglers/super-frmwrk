#include "req-parser.hpp"
#include "constants.hpp"
#include "request.hpp"

ReqParser::ReqParser() : req(std::make_unique<Request>()) {}

void ReqParser::parseReqLine(std::string str) {}

void ReqParser::parseHeaders(std::string str) {}

void ReqParser::parseHeaderSection(const std::string& str) {
    auto        firstSP  = str.find(' ');
    std::string path     = str.substr(0, firstSP);
    auto        secondSP = str.find(' ', firstSP + 1);
    std::string mt1      = str.substr(firstSP + 1, secondSP);

    MethodType mt2 = HTTP_GET;

    if (mt1 == "GET") {
        mt2 = MethodType::HTTP_GET;
    } else if (mt1 == "PUT") {
        mt2 = MethodType::HTTP_PUT;
    } else if (mt1 == "POST") {
        mt2 = MethodType::HTTP_POST;
    } else if (mt1 == "DELETE") {
        mt2 = MethodType::HTTP_DELETE;
    } else if (mt1 == "HEAD") {
        mt2 = MethodType::HTTP_HEAD;
    } else if (mt1 == "TRACE") {
        mt2 = MethodType::HTTP_TRACE;
    } else if (mt1 == "PATCH") {
        mt2 = MethodType::HTTP_PATCH;
    } else if (mt1 == "OPTIONS") {
        mt2 = MethodType::HTTP_OPTIONS;
    } else if (mt1 == "CONNECT") {
        mt2 = MethodType::HTTP_CONNECT;
    } else {
        mt2 = MethodType::HTTP_GET;
    }

    this->req->setPathAndType(PathAndType(path, mt2));
}

std::unique_ptr<Request> ReqParser::moveRequest() {
    return std::move(req);
}
