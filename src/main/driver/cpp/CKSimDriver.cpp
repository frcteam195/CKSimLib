#include "team195/CKSimDriver.h"

#include <iostream>
#include <stdlib.h>

namespace ck
{
    const std::string ZMQ_SERVER_IP = "127.0.0.1";
    //const std::string ZMQ_SERVER_IP = "10.1.95.199";
    const std::string ZMQ_REQREP_SERVER_PORT = "10501";
    const std::string ZMQ_PUBSUB_SERVER_PORT = "10502";
    // const std::string TOPIC = "10000";
    const std::string TOPIC = "";

    static void *contextSubscriber;
    static void *subscriberSocket;
    int ZMQSubInit()
    {
        int socketRecvTimeout = 1000;
        contextSubscriber = zmq_ctx_new();
        subscriberSocket = zmq_socket(contextSubscriber, ZMQ_SUB);
        int retVal = 0;
        if ((retVal = zmq_connect(subscriberSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_PUBSUB_SERVER_PORT).c_str())) > -1)
        {
            retVal = zmq_setsockopt(subscriberSocket, ZMQ_SUBSCRIBE, TOPIC.c_str(), TOPIC.length());
            retVal |= zmq_setsockopt(subscriberSocket, ZMQ_RCVTIMEO, &socketRecvTimeout, sizeof(int));
        }
        return retVal;
    }

    static void *contextRequester;
    static void *requesterSocket;
    int ZMQReqInit()
    {
        contextRequester = zmq_ctx_new();
        requesterSocket = zmq_socket(contextRequester, ZMQ_REQ);
        std::cout << "Req Socket Init: " << strerror(errno) << std::endl;
        int retVal = zmq_connect(requesterSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_REQREP_SERVER_PORT).c_str());
        std::cout << BuildConnStr(ZMQ_SERVER_IP, ZMQ_REQREP_SERVER_PORT).c_str() << std::endl;
        std::cout << "Req Connect: " << strerror(errno) << std::endl;
        std::cout << "Retval: " << retVal << std::endl;
        return retVal;
    }

    int ZMQSubRecv()
    {
        char buf[1000] = "";
        zmq_msg_t in_msg;
        zmq_msg_init(&in_msg);
        int numBytes = 0;
        if ((numBytes = zmq_recvmsg(subscriberSocket, &in_msg, ZMQ_NULL)) > -1)
        {
            if (int size = zmq_msg_size(&in_msg))
            {
                memcpy(buf, zmq_msg_data(&in_msg), size);
                zmq_msg_close(&in_msg);
                buf[size] = 0;
                std::cout << "Got " << numBytes << " bytes!" << std::endl;
                std::cout << "MsgSize " << size << " bytes!" << std::endl;
                std::cout << buf << std::endl;
            }
        }
        return numBytes;
    }

    void CKSimDealloc()
    {
        zmq_close(subscriberSocket);
        zmq_ctx_destroy(contextSubscriber);
        zmq_close(requesterSocket);
        zmq_ctx_destroy(contextRequester);
    }

    std::string BuildConnStr(std::string ipAddr, std::string port)
    {
        std::stringstream ss;
        ss << "tcp://" << ipAddr << ":" << port;
        return ss.str();
    }
} // namespace ck

extern "C"
{
    int c_CKSimDriver()
    {
        int retVal = ck::ZMQSubInit();
        //retVal |= ck::ZMQReqInit();
        return retVal;
    }

    void c_CKSimDealloc()
    {
        ck::CKSimDealloc();
    }

    int c_ZMQSubRecv()
    {
        return ck::ZMQSubRecv();
    }
} //extern "C"
