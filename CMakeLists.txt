cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
find_package(ZeroMQ REQUIRED 3)
include_directories(${ZEROMQ_INCLUDE_DIRS} common)

add_subdirectory(TopLeft)
add_subdirectory(TopRight)
add_subdirectory(BottomLeft)
add_subdirectory(BottomRight)
