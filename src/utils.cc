#include <chrono>
#include <filesystem>
#include <system_error>
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
