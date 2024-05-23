cmake_minimum_required(VERSION 3.22)
project(
    hfsaver
    VERSION 0.1.0
    DESCRIPTION "A Thread-safe library for saving http-flv streams to disk."
    HOMEPAGE_URL "https://github.com/zakiaatot/libhfsaver"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

# platform
if (WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
else()
    add_definitions(-DPLATFORM_OTHER)
endif()

# ffmpeg libs
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    include_directories(c:/ffmpeg/include)
    set(ENV{PKG_CONFIG_PATH} "C:\\ffmpeg\\lib\\pkgconfig")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
add_subdirectory(lib/FFmpeg)

# make sure we have all the required libraries
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

# jni
include_directories(${PROJECT_SOURCE_DIR}/jni)


add_library(${PROJECT_NAME}_shared SHARED ${SRC})
set_target_properties(
    ${PROJECT_NAME}_shared
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib
)
add_library(${PROJECT_NAME}_static STATIC ${SRC})
set_target_properties(
    ${PROJECT_NAME}_static
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/lib
)
target_link_libraries(${PROJECT_NAME}_shared FFmpeg)