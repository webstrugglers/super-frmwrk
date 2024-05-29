#include "logger.hpp"
#include <cstring>
#include <iostream>
#include <mutex>

std::mutex SafeLogger::mex;

// Safely prints provided message to std::cerr
void SafeLogger::log(const char* output) {
    std::unique_lock<std::mutex> lock(mex);
    std::cerr << output << std::endl;
}

void SafeLogger::log(const int _errnum) {
    std::unique_lock<std::mutex> lock(mex);
    std::cerr << strerror(_errnum) << std::endl;
}
