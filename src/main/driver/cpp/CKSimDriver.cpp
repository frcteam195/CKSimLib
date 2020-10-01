#include "team195/CKSimDriver.h"

#include <iostream>

namespace ck {
    std::string ZMQ_SERVER_IP = "127.0.0.1";
    // std::string ZMQ_SERVER_IP = "10.1.95.199";
    std::string ZMQ_REQREP_SERVER_PORT = "10501";
    std::string ZMQ_PUBSUB_SERVER_PORT = "10502";

    static void *contextSubscriber;
    static void *subscriberSocket;
    int ZMQSubInit() {
        contextSubscriber = zmq_ctx_new();
        subscriberSocket = zmq_socket(contextSubscriber, ZMQ_SUB);
        std::cout << "Sub Socket Init: " << strerror(errno) << std::endl;
        errno = 0;
        int retVal = zmq_connect(subscriberSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_PUBSUB_SERVER_PORT).c_str());
        std::cout << BuildConnStr(ZMQ_SERVER_IP, ZMQ_PUBSUB_SERVER_PORT).c_str() << std::endl;
        std::cout << "Sub Connect: " << strerror(errno) << std::endl;
        zmq_setsockopt(subscriberSocket, ZMQ_SUBSCRIBE, "", 0);
        errno = 0;
        std::cout << "Retval: " << retVal << std::endl;
        return retVal;
    }

    static void *contextRequester;
    static void *requesterSocket;
    int ZMQReqInit() {
        contextRequester = zmq_ctx_new();
        requesterSocket = zmq_socket(contextRequester, ZMQ_REQ);
        std::cout << "Req Socket Init: " << strerror(errno) << std::endl;
        int retVal = zmq_connect(requesterSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_REQREP_SERVER_PORT).c_str());
        std::cout << BuildConnStr(ZMQ_SERVER_IP, ZMQ_REQREP_SERVER_PORT).c_str() << std::endl;
        std::cout << "Req Connect: " << strerror(errno) << std::endl;
        std::cout << "Retval: " << retVal << std::endl;
        return retVal;
    }

    void CKSimDealloc() {
        zmq_close(subscriberSocket);
        zmq_ctx_destroy(contextSubscriber);
        zmq_close(requesterSocket);
        zmq_ctx_destroy(contextRequester);
    }

    std::string BuildConnStr(std::string ipAddr, std::string port) {
        std::stringstream ss;
        ss << "tcp://" << ipAddr << ":" << port;
        return ss.str();
    }
}

extern "C" {
   int c_CKSimDriver() {
       int retVal = ck::ZMQSubInit();
       retVal |= ck::ZMQReqInit();
       return retVal;
    }

    void c_CKSimDealloc() {
        ck::CKSimDealloc();
    }
} //extern "C"
