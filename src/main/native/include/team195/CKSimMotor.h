#pragma once

namespace team195
{
    class CKSimMotor
    {
    public:
        CKSimMotor(int id);
        float GetMotorValue();
        void SetMotorValue(float value);
        float GetEncoderValue();

    private:
        int id;
        bool validateId(int id);
    };
} // namespace team195