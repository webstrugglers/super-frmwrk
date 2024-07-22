#ifndef PATH_AND_TYPE_HPP
#define PATH_AND_TYPE_HPP

#include "constants.hpp"

/**
 * @struct PathAndType
 * @brief Encapsulates an http method and uri path.
 *
 * This struct is used to combine an http method (e.g., GET, POST) with a
 * uri path. Objects of this type are primarily used internally by components
 * such as the router and request classes.
 *
 */
using PathAndType = struct PathAndType {
    std::string path; /**< Represents path */

    MethodType method_type; /**< Represents HTTP method type */
    PathAndType();
    PathAndType(std::string p, const MethodType mt);
};

/**
 * @brief Compare 2 Objects of type PathAndType. Necessary for router
 */
bool operator==(const PathAndType& pat1, const PathAndType& pat2);

namespace std {
template <>
struct hash<PathAndType> {
    std::size_t operator()(const PathAndType& pat) const;
};
}  // namespace std

#endif  // PATH_AND_TYPE_HPP
