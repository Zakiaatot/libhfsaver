cmake_minimum_required(VERSION 3.22)
project(
    libhfsaver
    VERSION 0.1.0
    DESCRIPTION "A library for saving http-flv streams to disk."
    HOMEPAGE_URL "https://github.com/zakiaatot/libmterm"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

# ffmpeg libs
add_subdirectory(lib/FFmpeg)

# make sure we have all the required libraries
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

add_executable(
    ${PROJECT_NAME}
    ${SRC}
)
target_link_libraries(${PROJECT_NAME} FFmpeg)