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

constexpr char PAGE_NOT_FOUND_HTML[] = R"(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>404 Not Found</title>
    <style>
      body {
        margin: 0;
        padding: 0;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        background: #f3f4f6;
        color: #333;
      }
      .container {
        text-align: center;
      }
      .container h1 {
        font-size: 10em;
        margin: 0;
      }
      .container h2 {
        font-size: 2em;
        margin: 0.5em 0;
      }
      .container p {
        margin: 1em 0;
        font-size: 1.2em;
      }
      .container a {
        display: inline-block;
        margin-top: 1em;
        padding: 0.5em 1em;
        background: #ff12ba;
        color: white;
        text-decoration: none;
        border-radius: 0.3em;
        transition: background 0.3s;
      }
      .container a:hover {
        background: #ff12ba;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <h1>404</h1>
      <h2>Page Not Found</h2>
      <p>Sorry, the page you are looking for does not exist.</p>
      <a href="/">Go to Homepage</a>
    </div>
  </body>
</html>
)";

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
