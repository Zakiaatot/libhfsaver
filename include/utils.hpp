#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <string>

namespace Utils {
    unsigned long get_timestamp_ms();
    unsigned long get_file_size(const char* path);
    std::string seconds_to_time(const unsigned long seconds);
    void send_sigint(unsigned long pid);
};

#endif //!_UTILS_HPP_
