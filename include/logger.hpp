#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cerrno>
#include <mutex>
#include <string>

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
    static void log(const std::string& output);

    /// Safely prints provided message to std::cerr
    static void log(const int _errnum);
};

#endif  // !LOGGER_HPP
