cmake_minimum_required(VERSION 3.17)
project(proc)

set(CMAKE_CXX_STANDARD 20)

add_executable(proc main.cpp)

include_directories(src)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(proc ${CMAKE_PROJECT_NAME}_lib)