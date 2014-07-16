#include "sockets.h"

#include <zmq.h>

#include <string>
#include <cstring>
#include <cassert>

#include <iostream>

int main(int argc, const char *argv[])
{
    void *context = zmq_ctx_new();
    void *socket = createSocket(context, argv[1], ZMQ_PUB);

    std::string strWildcards = "*.hpp;*.h;*.cpp";

    while (1)
    {
        char c;
        std::cin >> c;
        if (c == 'q')
            break;

        zmq_msg_t list;
        zmq_msg_init_size(&list, strWildcards.size() + 1);
        memcpy(zmq_msg_data(&list), strWildcards.c_str(),
               strWildcards.size() + 1);

        zmq_msg_send(&list, socket, 0);

        zmq_msg_close(&list);
    }

    zmq_close(socket);

    zmq_ctx_destroy(context);
}
