cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH CMake)
find_library(ZeroMQ REQUIRED 3)
include_directories(${ZEROMQ_INCLUDE_DIRS})

add_subdirectory(TopLeft)
