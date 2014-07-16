#include "sockets.h"

#include <zmq.h>

#include <string>
#include <cassert>
#include <cstdlib>

void *createSocket(void *context, const char *port, int type)
{
    void *socket = zmq_socket(context, type);
    if (type == ZMQ_SUB)
    {
        int ret = zmq_connect(
            socket,
            (std::string("tcp://127.0.0.1:") + port).c_str());
        assert(ret == 0);
        ret = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
        assert(ret == 0);
    }
    else if (type == ZMQ_PUB)
    {
        int ret = zmq_bind(
            socket,
            (std::string("tcp://127.0.0.1:") + port).c_str());
        assert(ret == 0);
    }
    else
    {
        abort();
    }
    return socket;
}

int demultiplexSockets(const std::vector<void*> &sockets)
{
	const size_t iNumSockets = sockets.size();
    std::vector<zmq_pollitem_t> entries(iNumSockets);

	for (size_t s = 0; s < iNumSockets; ++s)
	{
		zmq_pollitem_t &entry = entries[s];
        entry.socket = sockets[s];
        entry.events = ZMQ_POLLIN;
    }

	int iPollResult = zmq_poll(entries.data(), iNumSockets, -1);
    assert(iPollResult > 0);

    //find first entry which has data available
    for(size_t e = 0; e < iNumSockets; ++e)
    {
        if(entries[e].revents & ZMQ_POLLIN)
            return e;
	}
}

void processDone(void *socket)
{
    zmq_msg_t done;
    zmq_msg_init(&done);
    zmq_msg_recv(&done, socket, 0);
    zmq_msg_close(&done);
}
