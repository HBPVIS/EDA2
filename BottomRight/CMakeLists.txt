add_executable(BottomRight BottomRight.cpp ../common/sockets.cpp)
target_link_libraries(BottomRight ${ZEROMQ_LIBRARIES})
