#pragma once

#if defined(__cplusplus) && __has_include(<array>)
#else
#error This project requires C++11 or greater
#endif


#define ZMQ_STATIC
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <string_view>
#include <sstream>
#include <errno.h>

namespace ck {
    extern std::string ZMQ_SERVER_IP;
    extern std::string ZMQ_REQREP_SERVER_PORT;
    extern std::string ZMQ_PUBSUB_SERVER_PORT;


    int ZMQSubInit();
    int ZMQReqInit();
    void CKSimDealloc();
    std::string BuildConnStr(std::string ipAddr, std::string port);
}

extern "C" {
    int c_CKSimDriver();
    void c_CKSimDealloc();
}  // extern "C"