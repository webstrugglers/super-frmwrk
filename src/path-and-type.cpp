#include "path-and-type.hpp"
#include <string>
#include "constants.hpp"

PathAndType::PathAndType() : path(""), method_type(GET) {}

PathAndType::PathAndType(const std::string& path, const MethodType method_type)
    : path(path),
      method_type(method_type) {}

PathAndType::PathAndType(const PathAndType& pat)
    : path(pat.path),
      method_type(pat.method_type) {}

std::string PathAndType::getPath() const {
    return this->path;
}

MethodType PathAndType::getMethodType() const {
    return this->method_type;
}

void PathAndType::setPath(const std::string& path) {
    this->path = path;
}

void PathAndType::setMethodType(const MethodType method_type) {
    this->method_type = method_type;
}

PathAndType& PathAndType::operator=(const PathAndType& pat) {
    if (this != &pat) {
        path        = pat.path;
        method_type = pat.method_type;
    }
    return *this;
}
