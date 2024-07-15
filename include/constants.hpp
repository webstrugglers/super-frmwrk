#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <ostream>

#define SOCKET_FD int

enum MethodType {
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE,
    HTTP_PATCH,
};

inline std::ostream& operator<<(std::ostream& os, const MethodType mt) {
    switch (mt) {
        case HTTP_GET:
            os << "GET";
            break;
        case HTTP_HEAD:
            os << "HEAD";
            break;
        case HTTP_POST:
            os << "POST";
            break;
        case HTTP_PUT:
            os << "PUT";
            break;
        case HTTP_DELETE:
            os << "DELETE";
            break;
        case HTTP_CONNECT:
            os << "CONNECT";
            break;
        case HTTP_OPTIONS:
            os << "OPTIONS";
            break;
        case HTTP_TRACE:
            os << "TRACE";
            break;
        case HTTP_PATCH:
            os << "PATCH";
            break;
    }
    return os;
}

inline std::string methodTypeToString(MethodType mt) {
    std::string ret;
    switch (mt) {
        case HTTP_GET:
            ret = "GET";
            break;
        case HTTP_HEAD:
            ret = "HEAD";
            break;
        case HTTP_POST:
            ret = "POST";
            break;
        case HTTP_PUT:
            ret = "PUT";
            break;
        case HTTP_DELETE:
            ret = "DELETE";
            break;
        case HTTP_CONNECT:
            ret = "CONNECT";
            break;
        case HTTP_OPTIONS:
            ret = "OPTIONS";
            break;
        case HTTP_TRACE:
            ret = "TRACE";
            break;
        case HTTP_PATCH:
            ret = "PATCH";
            break;
        default:
            ret = "UNKNOWN";
            break;
    }
    return ret;
}

inline std::string operator+(const std::string& str, const MethodType mt) {
    return str + methodTypeToString(mt);
}

#endif  // !CONSTANTS_HPP
