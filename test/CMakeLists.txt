cmake_minimum_required(VERSION 3.22)
project(
    hfsaver_test
    LANGUAGES C CXX
)

include_directories(${PROJECT_SOURCE_DIR}/../output/include)
link_directories(${PROJECT_SOURCE_DIR}/../output/lib)


# ffmpeg libs
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories(c:/ffmpeg/include)
    set(ENV{PKG_CONFIG_PATH} "C:\\ffmpeg\\lib\\pkgconfig")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
add_subdirectory(lib/FFmpeg)


add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} hfsaver_shared)


add_executable(transcode transcode.c)
target_link_libraries(transcode FFmpeg)

