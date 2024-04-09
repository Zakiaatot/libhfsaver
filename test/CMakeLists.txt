cmake_minimum_required(VERSION 3.22)
project(
    hfsaver_test
    LANGUAGES C CXX
)

include_directories(${PROJECT_SOURCE_DIR}/../output/include)
link_directories(${PROJECT_SOURCE_DIR}/../output/lib)


add_executable(${PROJECT_NAME} main.cc)
target_link_libraries(${PROJECT_NAME} hfsaver_shared)
