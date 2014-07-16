#ifndef COMMON_SOCKETS_H
#define COMMON_SOCKETS_H

#include <vector>

void *createSocket(void *context, const char *port, int type);

int demultiplexSockets(const std::vector<void*> &sockets);

void processDone(void *socket);

#endif



