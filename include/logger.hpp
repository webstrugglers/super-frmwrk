#include <cerrno>
#include <mutex>

/**
 * @class SafeLogger
 * @brief Thread safe logger
 *
 */
class SafeLogger {
private:
    static std::mutex mex;

public:
    /// Safely prints provided message to std::cerr
    static void log(const char* output);

    /// Safely prints provided message to std::cerr
    static void log(const int _errnum);
};
