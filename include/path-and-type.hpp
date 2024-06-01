#ifndef PATH_AND_TYPE_HPP
#define PATH_AND_TYPE_HPP

#include <functional>
#include <string>
#include "constants.hpp"

class PathAndType {
private:
    std::string path;
    MethodType  method_type;

public:
    explicit PathAndType(const std::string& path        = "/",
                         const MethodType   method_type = GET);
    PathAndType(const PathAndType& pat);

    std::string getPath() const;
    MethodType  getMethodType() const;

    void setPath(const std::string& path);
    void setMethodType(const MethodType method_type);

    PathAndType& operator=(const PathAndType& pat);
    bool         operator==(const PathAndType& pat) const;
};

namespace std {
template <>
struct hash<PathAndType> {
    std::size_t operator()(const PathAndType& pat) const;
};
}  // namespace std

#endif  // PATH_AND_TYPE_HPP
