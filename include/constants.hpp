#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstring>
#include <sstream>
#include <string>
#define SOCKET_FD int
#define REQ_BUF_SIZE 4096

enum HttpStatus {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS,
    PROCESSING,
    EARLY_HINTS,

    OK = 200,
    CREATED,
    ACCEPTED,
    NON_AUTHORITATIVE_INFORMATION,
    NO_CONTENT,
    RESET_CONTENT,
    PARTIAL_CONTENT,
    MULTI_STATUS,
    ALREADY_REPORTED,
    IM_USED = 226,

    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    NOT_MODIFIED,
    TEMPORARY_REDIRECT = 307,
    PERMANENT_REDIRECT,

    BAD_REQUEST = 400,
    UNAUTHORIZED,
    PAYMENT_REQUIRED,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    PROXY_AUTHENTICATION_REQUIRED,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PRECONDITION_FAILED,
    PAYLOAD_TOO_LARGE,
    URI_TOO_LONG,
    UNSUPPORTED_MEDIA_TYPE,
    RANGE_NOT_SATISFIABLE,
    EXPECTATION_FAILED,
    IM_A_TEAPOT,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_CONTENT,
    LOCKED,
    FAILED_DEPENDENCY,
    TOO_EARLY,
    UPGRADE_REQUIRED,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASON    = 451,

    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIMEOUT,
    HTTP_VERSION_NOT_SUPPORTED,
    VARIANT_ALSO_NEGOTIATES,
    INSUFFICIENT_STORAGE,
    LOOP_DETECTED,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED,
};

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

inline std::string operator+(std::string str, const MethodType mt) {
    std::ostringstream stream;
    stream << mt;
    return str + stream.str();
}

/*//  @brief  Concatenate C string and MethodType.*/
/*inline char* operator+(char* cstr, const MethodType mt) {*/
/*    std::strcat(cstr, methodTypeToString(mt).c_str());*/
/*    return cstr;*/
/*}*/
/**/
/*//  @brief  Concatenate MethodType and C string.*/
/*inline char* operator+(MethodType mt, const char* cstr) {*/
/*    std::string mt_str = methodTypeToString(mt);*/
/*    mt_str += cstr;*/
/**/
/*    /*return mt_str.c_str();*/
/*}*/

#endif  // !CONSTANTS_HPP
