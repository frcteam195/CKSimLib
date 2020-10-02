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
#include <hal/HAL.h>
#include <wpi/mutex.h>
#include <wpi/SafeThread.h>

#include "team195/globalconf.h"
#include "protobuf/ControlMessage.pb.h"
#include "protobuf/StatusMessage.pb.h"
#include "protobuf/ValueMessage.pb.h"

namespace ck
{
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
        int socketRecvTimeout = 1000;
        contextRequester = zmq_ctx_new();
        requesterSocket = zmq_socket(contextRequester, ZMQ_REQ);
        int retVal = 0;
        if ((retVal = zmq_connect(requesterSocket, BuildConnStr(ZMQ_SERVER_IP, ZMQ_REQREP_SERVER_PORT).c_str())) > -1)
        {
            retVal = zmq_setsockopt(requesterSocket, ZMQ_SNDTIMEO, &socketRecvTimeout, sizeof(int));
            retVal |= zmq_setsockopt(requesterSocket, ZMQ_RCVTIMEO, &socketRecvTimeout, sizeof(int));
        }
        return retVal;
    }

    int ZMQSubRecvTest()
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

    static wpi::mutex controlMsgMutex;
    static ControlMessage controlMessage;
    void ControlMessageInit()
    {
        std::lock_guard<wpi::mutex> lock(controlMsgMutex);
        for (int i = 0; i < MAX_NUM_MOTORS; i++)
        {
            ValueMessage *v = controlMessage.add_motors();
            v->set_id(i);
            v->set_value(0);
        }
    }
    float GetMotor(int id)
    {
        std::lock_guard<wpi::mutex> lock(controlMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            return controlMessage.motors(id).value();
        }
        return 0;
    }
    void SetMotor(int id, float val)
    {
        std::lock_guard<wpi::mutex> lock(controlMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            controlMessage.mutable_motors(id)->set_value(val);
        }
    }

    static wpi::mutex statusMsgMutex;
    static StatusMessage statusMessage;
    void StatusMessageInit()
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        for (int i = 0; i < MAX_NUM_MOTORS; i++)
        {
            ValueMessage *v = statusMessage.add_encoders();
            v->set_id(i);
            v->set_value(0);
        }
        for (int i = 0; i < MAX_NUM_ACCEL; i++)
        {
            ValueMessage *v = statusMessage.add_accelerometers();
            v->set_id(i);
            v->set_value(0);
        }
        for (int i = 0; i < MAX_NUM_GYRO; i++)
        {
            ValueMessage *v = statusMessage.add_gyroscopes();
            v->set_id(i);
            v->set_value(0);
        }
    }
    float GetEncoder(int id)
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            return statusMessage.encoders(id).value();
        }
        return 0;
    }
    void SetEncoder(int id, float val)
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_MOTORS)
        {
            statusMessage.mutable_encoders(id)->set_value(val);
        }
    }
    float GetAccelerometer(int id)
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_ACCEL)
        {
            return statusMessage.accelerometers(id).value();
        }
        return 0;
    }
    void SetAccelerometer(int id, float val)
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_ACCEL)
        {
            statusMessage.mutable_accelerometers(id)->set_value(val);
        }
    }
    float GetGyro(int id)
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_GYRO)
        {
            return statusMessage.gyroscopes(id).value();
        }
        return 0;
    }
    void SetGyro(int id, float val)
    {
        std::lock_guard<wpi::mutex> lock(statusMsgMutex);
        if (id >= 0 && id < MAX_NUM_GYRO)
        {
            statusMessage.mutable_gyroscopes(id)->set_value(val);
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
            while (m_active)
            {
                std::unique_lock<wpi::mutex> lock(controlMsgMutex);
                if (!controlMessage.SerializeToArray(bufSendArr, DATA_BUF_BYTES))
                {
                    continue;
                }
                dataSize = controlMessage.ByteSizeLong();
                lock.unlock();

                zmq_send(requesterSocket, bufSendArr, dataSize, ZMQ_NULL);
                zmq_recv(requesterSocket, bufRecvArr, DATA_BUF_BYTES_SMALL, ZMQ_NULL);
                currTime = HAL_GetFPGATime(&status);
                HAL_UpdateNotifierAlarm(m_setNotifier, (uint64_t)(currTime + (kThreadRate - (currTime - startTime))), &status);
                HAL_WaitForNotifierAlarm(m_setNotifier, &status);
                startTime = HAL_GetFPGATime(&status);
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
            while (m_active)
            {
                int dataSize = 0;
                if ((dataSize = zmq_recv(subscriberSocket, bufRecvArr, DATA_BUF_BYTES, ZMQ_NULL)) > 0)
                {
                    StatusMessage tmpStatusMessage;
                    if (tmpStatusMessage.ParseFromArray(bufRecvArr, dataSize))
                    {
                        std::unique_lock<wpi::mutex> lock(statusMsgMutex);
                        statusMessage = tmpStatusMessage;
                        lock.unlock();
                    }
                }
            }
        }
    };

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
    static wpi::SafeThreadOwner<ck::CommDaemonSend> m_CommDaemonSend;
    static wpi::SafeThreadOwner<ck::CommDaemonRecv> m_CommDaemonRecv;
    static wpi::mutex driverMutex;
    static uint8_t initializedCommDaemon = 0;
    int c_CKSimDriver()
    {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        std::lock_guard<wpi::mutex> lock(driverMutex);
        ck::ControlMessageInit();
        ck::StatusMessageInit();
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
        std::lock_guard<wpi::mutex> lock(driverMutex);
        m_CommDaemonSend.Stop();
        m_CommDaemonRecv.Stop();
        m_CommDaemonSend.Join();
        m_CommDaemonRecv.Join();
        ck::CKSimDealloc();
    }

    int c_ZMQSubRecvTest()
    {
        std::lock_guard<wpi::mutex> lock(driverMutex);
        return ck::ZMQSubRecvTest();
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
} //extern "C"
