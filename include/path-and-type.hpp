#ifndef PATH_AND_TYPE_HPP
#define PATH_AND_TYPE_HPP

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
};

#endif  // PATH_AND_TYPE_HPP
