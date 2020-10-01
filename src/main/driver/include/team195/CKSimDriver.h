#pragma once

#if defined(__cplusplus) && __has_include(<array>)
#else
#error This project requires C++11 or greater
#endif

#include <string>
#include <errno.h>

namespace ck
{
    int ZMQSubInit();
    int ZMQReqInit();
    int ZMQSubRecvTest();
    void CKSimDealloc();
    std::string BuildConnStr(std::string ipAddr, std::string port);
} // namespace ck

extern "C"
{
    int c_CKSimDriver();
    void c_CKSimDealloc();
    int c_ZMQSubRecvTest();
} // extern "C"