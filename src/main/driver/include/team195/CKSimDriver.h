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
    void ControlMessageInit();
    float GetMotor(int id);
    void SetMotor(int id, float val);
    void StatusMessageInit();
    float GetEncoder(int id);
    void SetEncoder(int id, float val);
    float GetAccelerometer(int id);
    void SetAccelerometer(int id, float val);
    float GetGyro(int id);
    void SetGyro(int id, float val);
    void CKSimDealloc();
    std::string BuildConnStr(std::string ipAddr, std::string port);
} // namespace ck

extern "C"
{
    int c_CKSimDriver();
    void c_CKSimDealloc();
    int c_ZMQSubRecvTest();
    float c_GetMotor(int id);
    void c_SetMotor(int id, float val);
    float c_GetEncoder(int id);
    void c_SetEncoder(int id, float val);
    float c_GetAccelerometer(int id);
    void c_SetAccelerometer(int id, float val);
    float c_GetGyro(int id);
    void c_SetGyro(int id, float val);
} // extern "C"