#include "sockets.h"

#include <zmq.h>

#include <string>
#include <cstring>
#include <cassert>

#include <iostream>

#include <stdlib.h>

void processFilesNames(void *inSocket,
                       void *outSocket)
{
    zmq_msg_t filenames;
    zmq_msg_init(&filenames);
    zmq_msg_recv(&filenames, inSocket, 0);

    std::cout << "Recieve" << std::endl;
    std::string filenamesStr((char*)zmq_msg_data(&filenames));
    std::cout << filenamesStr << std::endl;

    size_t size = 5 * (1 << 20);
    zmq_msg_t files;
    zmq_msg_init_size(&files, size);
    char *data = static_cast<char*>(zmq_msg_data(&files));
    for (size_t i = 0; i != size; ++i)
    {
        if(i%1<<7)
            data[i] = 'f';
        else
            data[i] = static_cast<char>(random());
    }
    zmq_msg_send(&files, outSocket, 0);
    zmq_msg_close(&files);
}

int main(int argc, const char *argv[])
{
    void *context = zmq_ctx_new();

    assert(argc == 4);
    void *subFilenamesSock = createSocket(context, argv[1], ZMQ_SUB);
    void *pubFilesSock = createSocket(context, argv[2], ZMQ_PUB);
    void *subDoneSock = createSocket(context, argv[3], ZMQ_SUB);

    bool done = false;
    std::vector<void*> sockets;
    sockets.push_back(subFilenamesSock);
    sockets.push_back(subDoneSock);
    while (!done)
    {
        switch (demultiplexSockets(sockets))
        {
        case 0:
            processFilesNames(subFilenamesSock, pubFilesSock);
            break;
        case 1:
            processDone(subDoneSock);
            done = true;
            break;
        }
    }
}
