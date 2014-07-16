#include "sockets.h"

#include <zmq.h>

#include <cstring>
#include <iostream>
#include <cassert>

void processWildcards(void *inSocket, void *outSocket)
{
    zmq_msg_t wildcards;
    zmq_msg_init(&wildcards);
    zmq_msg_recv(&wildcards, inSocket, 0);

    std::cout << "Recieve" << std::endl;
    std::string wildcardsStr((char*)zmq_msg_data(&wildcards));
    std::cout << wildcardsStr << std::endl;

    zmq_msg_close(&wildcards);

    std::string strFilenames("{*.hpp:;*.h:;*.cpp:main.cpp}");
    zmq_msg_t filenames;
    zmq_msg_init_size(&filenames, strFilenames.size() + 1);
    memcpy(zmq_msg_data(&filenames), strFilenames.c_str(),
           strFilenames.size() + 1);

    zmq_msg_send(&filenames, outSocket, 0);

    zmq_msg_close(&filenames);
}

int main(int argc, const char *argv[])
{
    void *context = zmq_ctx_new();

    assert(argc == 4);
    void *subWildcardSock = createSocket(context, argv[1], ZMQ_SUB);
    void *pubFilenamesSock = createSocket(context, argv[2], ZMQ_PUB);
    void *subDoneSock = createSocket(context, argv[3], ZMQ_SUB);

    std::cout << "Sockets connected" << std::endl;

    bool done = false;
    std::vector<void*> sockets;
    sockets.push_back(subWildcardSock);
    sockets.push_back(subDoneSock);
    while (!done)
    {
        switch (demultiplexSockets(sockets))
        {
        case 0:
            processWildcards(subWildcardSock, pubFilenamesSock);
            break;
        case 1:
            processDone(subDoneSock);
            done = true;
            break;
        };
    }

    zmq_close(subWildcardSock);
    zmq_close(pubFilenamesSock);
    zmq_close(subDoneSock);
}
