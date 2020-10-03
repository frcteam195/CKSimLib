#pragma once

#include <string>

#define MAX_NUM_MOTORS 64
#define MAX_NUM_ACCEL 64
#define MAX_NUM_GYRO 64
#define MAX_NUM_ADVOBJ 64

constexpr int DATA_BUF_BYTES = 4096;
constexpr int DATA_BUF_BYTES_SMALL = 128;

namespace ck
{
    extern const std::string ZMQ_SERVER_IP;
    extern const std::string ZMQ_REQREP_SERVER_PORT;
    extern const std::string ZMQ_PUBSUB_SERVER_PORT;
    extern const std::string TOPIC;
} // namespace ck