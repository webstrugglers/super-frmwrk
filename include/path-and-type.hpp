#ifndef PATH_AND_TYPE_HPP
#define PATH_AND_TYPE_HPP

#include <functional>
#include <string>
#include "constants.hpp"

/**
 * @class PathAndType
 * @brief Encapsulates an HTTP method and URI path.
 *
 * This class is used to combine an HTTP method (e.g., GET, POST) with a URI
 * path. Objects of this type are primarily used internally by components such
 * as the Router and Request classes.
 */
class PathAndType {
private:
    std::string path;
    MethodType  method_type;

public:
    explicit PathAndType(const std::string& new_path        = "/",
                         const MethodType   new_method_type = HTTP_GET);

    std::string getPath() const;
    MethodType  getMethodType() const;

    void setPath(const std::string& new_path);
    void setMethodType(const MethodType new_method_type);

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
