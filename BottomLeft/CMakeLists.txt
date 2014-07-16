add_executable(BottomLeft BottomLeft.cpp ../common/sockets.cpp)
target_link_libraries(BottomLeft ${ZEROMQ_LIBRARIES})
