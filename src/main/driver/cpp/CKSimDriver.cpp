#include "team195/CKSimDriver.h"

namespace ck {
    std::string ZMQ_SERVER_IP = "10.1.95.199";
    std::string ZMQ_REQREP_SERVER_PORT = "10501";
    std::string ZMQ_PUBSUB_SERVER_PORT = "10502";

    static void *contextSubscriber;
    static void *subscriberSocket;
    int ZMQSubInit() {
        contextSubscriber = zmq_ctx_new();
        subscriberSocket = zmq_socket(contextSubscriber, ZMQ_REQ);
        return zmq_connect(subscriberSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_PUBSUB_SERVER_PORT).c_str());
    }

    static void *contextRequester;
    static void *requesterSocket;
    int ZMQReqInit() {
        contextRequester = zmq_ctx_new();
        requesterSocket = zmq_socket(contextRequester, ZMQ_REQ);
        return zmq_connect(requesterSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_REQREP_SERVER_PORT).c_str());
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
       return ck::ZMQSubInit() | ck::ZMQReqInit();
    }

    void c_CKSimDealloc() {
        ck::CKSimDealloc();
    }
} //extern "C"
