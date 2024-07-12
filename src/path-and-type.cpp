/**
 * @file path-and-type.cpp
 * @brief This file implements PathAndType class
 *
 */

#include "path-and-type.hpp"

PathAndType::PathAndType() : path("/"), method_type(HTTP_GET) {}

PathAndType::PathAndType(std::string p, const MethodType mt)
    : path(std::move(p)),
      method_type(mt) {}

bool operator==(const PathAndType& pat1, const PathAndType& pat2) {
    return (pat1.path == pat2.path) && (pat1.method_type == pat2.method_type);
}

namespace std {
std::size_t hash<PathAndType>::operator()(const PathAndType& pat) const {
    std::hash<std::string> stringHasher;
    std::hash<MethodType>  enumHasher;
    std::size_t            pathHash       = stringHasher(pat.path);
    std::size_t            methodTypeHash = enumHasher(pat.method_type);
    return pathHash ^ (methodTypeHash << 1);
}
}  // namespace std
