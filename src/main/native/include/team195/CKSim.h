#pragma once

#include "CKSingleton.h"
#include <string>

namespace team195
{
    class CKSim : public CKSingleton<CKSim>
    {
        friend CKSingleton;

    public:
        static constexpr int MAX_NUM_MOTORS = 64;
        static constexpr int MAX_NUM_ACCEL = 64;
        static constexpr int MAX_NUM_GYRO = 64;
        static constexpr int MAX_NUM_ADVOBJ = 64;
        ~CKSim();

        static void SetIP(std::string ip);

        bool registerMotor(int id);
        bool containsMotor(int id);

        bool registerAccelerometer(int id);
        bool containsAccelerometer(int id);

        bool registerGyro(int id);
        bool containsGyro(int id);

        bool registerAdvObj(int id);
        bool containsAdvObj(int id);

        float getMotorValue(int id);
        void setMotorValue(int id, float val);

        float getEncoderValue(int id);
        void setEncoderValue(int id, float val);

        float getAccelerometerValue(int id);
        void setAccelerometerValue(int id, float val);

        float getGyroValue(int id);
        void setGyroValue(int id, float val);

        float getAdvObjValue(int id);
        void setAdvObjValue(int id, float val);

    private:
        CKSim();
    };
} // namespace team195
