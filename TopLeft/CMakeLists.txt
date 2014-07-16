add_executable(TopLeft TopLeft.cpp ../common/sockets.cpp)
target_link_libraries(TopLeft ${ZEROMQ_LIBRARIES})
