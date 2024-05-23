#include <chrono>
#include <filesystem>
#include <system_error>
#include <sstream>
#include <iomanip>
#include "utils.hpp"

unsigned long Utils::get_timestamp_ms()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return static_cast<unsigned long>(millis);
}

unsigned long Utils::get_file_size(const char* path)
{
    std::error_code ec;
    std::uintmax_t file_size = std::filesystem::file_size(path, ec);
    if (ec)
    {
        return 0;
    }
    else
    {
        return file_size;
    }
}


std::string Utils::seconds_to_time(unsigned long seconds) {
    unsigned long hours = seconds / 3600;
    unsigned long minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":";
    ss << std::setw(2) << std::setfill('0') << minutes << ":";
    ss << std::setw(2) << std::setfill('0') << seconds;

    return ss.str();
}
