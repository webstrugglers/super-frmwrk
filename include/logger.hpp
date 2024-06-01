#include <cerrno>
#include <mutex>

// @brief thread safe logger
class SafeLogger {
private:
    static std::mutex mex;

public:
    static void log(const char* output);

    static void log(const int _errnum);
};
