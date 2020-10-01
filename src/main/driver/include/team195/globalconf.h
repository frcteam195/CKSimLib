#pragma once

#include <string>

#define MAX_NUM_MOTORS 64
#define MAX_NUM_ACCEL 64
#define MAX_NUM_GYRO 64


constexpr int DATA_BUF_BYTES = 1024;

namespace ck
{
    extern const std::string ZMQ_SERVER_IP;
    extern const std::string ZMQ_REQREP_SERVER_PORT;
    extern const std::string ZMQ_PUBSUB_SERVER_PORT;
    extern const std::string TOPIC;
} // namespace ck