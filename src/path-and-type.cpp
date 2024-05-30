#include "path-and-type.hpp"
#include "constants.hpp"

PathAndType::PathAndType() : path("/"), method_type(GET) {}

PathAndType::PathAndType(const std::string& path, const MethodType method_type)
    : path(path),
      method_type(method_type) {}

PathAndType::PathAndType(const PathAndType& pat) = default;

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

bool PathAndType::operator==(const PathAndType& pat) const {
    return (this->method_type == pat.method_type) && (this->path == pat.path);
}

namespace std {
std::size_t hash<PathAndType>::operator()(const PathAndType& pat) const {
    std::hash<std::string> stringHasher;
    std::hash<MethodType>  enumHasher;
    std::size_t            pathHash       = stringHasher(pat.getPath());
    std::size_t            methodTypeHash = enumHasher(pat.getMethodType());
    return pathHash ^ (methodTypeHash << 1);
}
}  // namespace std
