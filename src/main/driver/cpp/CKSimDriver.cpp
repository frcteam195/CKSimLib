#include "team195/CKSimDriver.h"

#define ZMQ_STATIC
#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <hal/HAL.h>
#include <wpi/mutex.h>
#include <wpi/SafeThread.h>

#include "team195/globalconf.h"
#include "protobuf/ControlMessage.pb.h"
#include "protobuf/StatusMessage.pb.h"
#include "protobuf/ValueMessage.pb.h"

namespace ck
{
    static std::string localSetIP;
    std::string GetIP()
    {
        if (localSetIP.empty())
        {
            std::cout << "Default IP Used (127.0.0.1)" << std::endl;
            return ZMQ_SERVER_IP_DEFAULT;
        }
        else
        {
            return localSetIP;
        }
    }
    void SetIP(std::string ip)
    {
        localSetIP = ip;
    }

    static void *contextZMQ;
    static void *subscriberSocket;
    int ZMQSubInit()
    {
        if (contextZMQ == NULL)
        {
            contextZMQ = zmq_ctx_new();
        }

        zmq_close(subscriberSocket);
        subscriberSocket = NULL;

        int socketRecvTimeout = 1000;
        subscriberSocket = zmq_socket(contextZMQ, ZMQ_SUB);
        int retVal = 0;
        if ((retVal = zmq_connect(subscriberSocket, BuildConnStr(GetIP(), ZMQ_PUBSUB_SERVER_PORT).c_str())) > -1)
        {
            retVal = zmq_setsockopt(subscriberSocket, ZMQ_SUBSCRIBE, TOPIC.c_str(), TOPIC.length());
            retVal |= zmq_setsockopt(subscriberSocket, ZMQ_RCVTIMEO, &socketRecvTimeout, sizeof(int));
        }
        else
        {
            subscriberSocket = NULL;
        }
        return retVal;
    }

    static void *requesterSocket;
    int ZMQReqInit()
    {
        if (contextZMQ == NULL)
        {
            contextZMQ = zmq_ctx_new();
        }

        zmq_close(requesterSocket);
        requesterSocket = NULL;

        int socketRecvTimeout = 1000;
        requesterSocket = zmq_socket(contextZMQ, ZMQ_REQ);
        int retVal = 0;
        if ((retVal = zmq_connect(requesterSocket, BuildConnStr(GetIP(), ZMQ_REQREP_SERVER_PORT).c_str())) > -1)
        {
            //retVal = zmq_setsockopt(requesterSocket, ZMQ_SNDTIMEO, &socketRecvTimeout, sizeof(int));
            //retVal |= zmq_setsockopt(requesterSocket, ZMQ_RCVTIMEO, &socketRecvTimeout, sizeof(int));
        }
        else
        {
            requesterSocket = NULL;
        }
        return retVal;
    }

    static std::mutex controlMsgMutex;
    static ControlMessage controlMessage;
    void ControlMessageInit()
    {
        std::lock_guard<std::mutex> lock(controlMsgMutex);
        for (int i = 0; i < MAX_NUM_MOTORS; i++)
        {
            ValueMessage *v = controlMessage.add_motors();
            v->set_id(i);
            v->set_value(0);
        }
    }
    float GetMotor(int id)
    {
        std::lock_guard<std::mutex> lock(controlMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            return controlMessage.motors(id).value();
        }
        return 0;
    }
    void SetMotor(int id, float val)
    {
        std::lock_guard<std::mutex> lock(controlMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            controlMessage.mutable_motors(id)->set_value(val);
        }
    }

    static std::map<int, float> encoderMap;
    static std::map<int, float> accelMap;
    static std::map<int, float> gyroMap;
    static std::map<int, float> advObjMap;
    static std::mutex statusMsgMutex;
    void StatusMapsInit()
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        for (int i = 0; i < MAX_NUM_MOTORS; i++)
        {
            encoderMap[i] = 0;
        }
        for (int i = 0; i < MAX_NUM_ACCEL; i++)
        {
            accelMap[i] = 0;
        }
        for (int i = 0; i < MAX_NUM_GYRO; i++)
        {
            gyroMap[i] = 0;
        }
        for (int i = 0; i < MAX_NUM_ADVOBJ; i++)
        {
            advObjMap[i] = 0;
        }
    }
    float GetEncoder(int id)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            return encoderMap[id];
        }
        return 0;
    }
    void SetEncoder(int id, float val)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            encoderMap[id] = val;
        }
    }
    float GetAccelerometer(int id)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_ACCEL)
        {
            return accelMap[id];
        }
        return 0;
    }
    void SetAccelerometer(int id, float val)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_ACCEL)
        {
            accelMap[id] = val;
        }
    }
    float GetGyro(int id)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_GYRO)
        {
            return gyroMap[id] = 0;
        }
        return 0;
    }
    void SetGyro(int id, float val)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_GYRO)
        {
            gyroMap[id] = val;
        }
    }
    float GetAdvObj(int id)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_ADVOBJ)
        {
            return advObjMap[id] = 0;
        }
        return 0;
    }
    void SetAdvObj(int id, float val)
    {
        std::lock_guard<std::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_ADVOBJ)
        {
            advObjMap[id] = val;
        }
    }

    class CommDaemonSend : public wpi::SafeThread
    {
    public:
        CommDaemonSend() {}
        ~CommDaemonSend() {}

    private:
        void Main() override
        {
            constexpr uint64_t kThreadRate = 10000; //10ms, units in usec
            int32_t status;
            HAL_NotifierHandle m_setNotifier = HAL_InitializeNotifier(&status);
            uint64_t currTime;
            uint64_t startTime = HAL_GetFPGATime(&status);
            char bufRecvArr[DATA_BUF_BYTES_SMALL];
            char bufSendArr[DATA_BUF_BYTES];
            memset(bufRecvArr, 0, sizeof(bufRecvArr));
            memset(bufSendArr, 0, sizeof(bufSendArr));
            size_t dataSize = 0;
            unsigned int requesterFailCounter = 0;
            while (m_active)
            {
                if (requesterSocket != NULL)
                {
                    std::unique_lock<std::mutex> lock(controlMsgMutex);
                    if (!controlMessage.SerializeToArray(bufSendArr, DATA_BUF_BYTES))
                    {
                        continue;
                    }
                    dataSize = controlMessage.ByteSizeLong();
                    lock.unlock();

                    int retVal = zmq_send(requesterSocket, bufSendArr, dataSize, ZMQ_NULL);
                    // std::cout << "ZMQSend retval: " << retVal << std::endl;
                    if (retVal < 0)
                    {
                        // std::cout << "ZMQSend Error: " << strerror(errno) << std::endl;
                    }
                    int retBytes = zmq_recv(requesterSocket, bufRecvArr, DATA_BUF_BYTES_SMALL, ZMQ_NOBLOCK);
                    if (retBytes <= 0)
                    {
                        ++requesterFailCounter %= 10;
                        if (requesterFailCounter == 0)
                        {
                            requesterSocket = NULL;
                        }
                    }
                    currTime = HAL_GetFPGATime(&status);
                    HAL_UpdateNotifierAlarm(m_setNotifier, (uint64_t)(currTime + (kThreadRate - (currTime - startTime))), &status);
                    HAL_WaitForNotifierAlarm(m_setNotifier, &status);
                    startTime = HAL_GetFPGATime(&status);
                    // std::cout << "Send Thread Running" << std::endl;
                }
                else
                {
                    ++requesterFailCounter %= 100;
                    if (requesterFailCounter == 0)
                    {
                        ZMQReqInit();
                    }
                }
            }
            HAL_StopNotifier(m_setNotifier, &status);
            HAL_CleanNotifier(m_setNotifier, &status);
        }
    };

    class CommDaemonRecv : public wpi::SafeThread
    {
    public:
        CommDaemonRecv() {}
        ~CommDaemonRecv() {}

    private:
        void Main() override
        {
            char bufRecvArr[DATA_BUF_BYTES];
            memset(bufRecvArr, 0, sizeof(bufRecvArr));
            unsigned int subscriberFailCounter = 0;
            while (m_active)
            {
                if (subscriberSocket != NULL)
                {
                    int dataSize = 0;
                    if ((dataSize = zmq_recv(subscriberSocket, bufRecvArr, DATA_BUF_BYTES, ZMQ_NULL)) > 0)
                    {
                        StatusMessage tmpStatusMessage;
                        if (tmpStatusMessage.ParseFromArray(bufRecvArr, dataSize))
                        {
                            std::unique_lock<std::mutex> lock(statusMsgMutex);
                            for (int i = 0; i < tmpStatusMessage.encoders_size(); i++)
                            {
                                const ValueMessage *tmpObj = &tmpStatusMessage.encoders(i);
                                encoderMap[tmpObj->id()] = tmpObj->value();
                            }
                            for (int i = 0; i < tmpStatusMessage.accelerometers_size(); i++)
                            {
                                const ValueMessage *tmpObj = &tmpStatusMessage.accelerometers(i);
                                accelMap[tmpObj->id()] = tmpObj->value();
                            }
                            for (int i = 0; i < tmpStatusMessage.gyroscopes_size(); i++)
                            {
                                const ValueMessage *tmpObj = &tmpStatusMessage.gyroscopes(i);
                                gyroMap[tmpObj->id()] = tmpObj->value();
                            }
                            for (int i = 0; i < tmpStatusMessage.advanced_size(); i++)
                            {
                                const ValueMessage *tmpObj = &tmpStatusMessage.advanced(i);
                                advObjMap[tmpObj->id()] = tmpObj->value();
                            }
                            lock.unlock();
                        }
                    }
                }
                else
                {
                    ++subscriberFailCounter %= 100;
                    if (subscriberFailCounter == 0)
                    {
                        ZMQSubInit();
                    }
                }
            }
        }
    };

    void CKSimDealloc()
    {
        zmq_close(subscriberSocket);
        zmq_close(requesterSocket);
        zmq_ctx_destroy(contextZMQ);
        subscriberSocket = NULL;
        requesterSocket = NULL;
        contextZMQ = NULL;
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
    static wpi::SafeThreadOwner<ck::CommDaemonSend> m_CommDaemonSend;
    static wpi::SafeThreadOwner<ck::CommDaemonRecv> m_CommDaemonRecv;
    static std::mutex driverMutex;
    static uint8_t initializedCommDaemon = 0;
    int c_CKSimDriver()
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        std::lock_guard<std::mutex> lock(driverMutex);
        ck::ControlMessageInit();
        ck::StatusMapsInit();
        ck::contextZMQ = zmq_ctx_new();
        int initVal = 0;
        if ((initVal = (ck::ZMQSubInit() | ck::ZMQReqInit())) < 0)
        {
            return -1;
        }

        if (!initializedCommDaemon)
        {
            m_CommDaemonSend.Start();
            m_CommDaemonRecv.Start();
            initializedCommDaemon++;
        }

        return initVal;
    }

    void c_CKSimDealloc()
    {
        std::lock_guard<std::mutex> lock(driverMutex);
        m_CommDaemonSend.Stop();
        m_CommDaemonRecv.Stop();
        m_CommDaemonSend.Join();
        m_CommDaemonRecv.Join();
        ck::CKSimDealloc();
    }

    void c_CKSimSetIP(const char *ip)
    {
        //Fix strings that come in not null terminated (potentially JNI)
        // char tmpIP[ipStrLen + 1];
        // std::memcpy(tmpIP, ip, ipStrLen);
        // tmpIP[ipStrLen] = 0;
        std::cout << std::endl
                  << ip << std::endl;
        ck::SetIP(std::string(ip));
    }

    float c_GetMotor(int id)
    {
        return ck::GetMotor(id);
    }
    void c_SetMotor(int id, float val)
    {
        ck::SetMotor(id, val);
    }
    float c_GetEncoder(int id)
    {
        return ck::GetEncoder(id);
    }
    void c_SetEncoder(int id, float val)
    {
        ck::SetEncoder(id, val);
    }
    float c_GetAccelerometer(int id)
    {
        return ck::GetAccelerometer(id);
    }
    void c_SetAccelerometer(int id, float val)
    {
        ck::SetAccelerometer(id, val);
    }
    float c_GetGyro(int id)
    {
        return ck::GetGyro(id);
    }
    void c_SetGyro(int id, float val)
    {
        ck::SetGyro(id, val);
    }
    float c_GetAdvObj(int id)
    {
        return ck::GetAdvObj(id);
    }
    void c_SetAdvObj(int id, float val)
    {
        ck::SetAdvObj(id, val);
    }
} //extern "C"
