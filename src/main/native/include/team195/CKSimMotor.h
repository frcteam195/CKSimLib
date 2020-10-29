#pragma once

#include <frc/smartdashboard/Sendable.h>
#include "frc/smartdashboard/SendableBuilder.h"

namespace team195
{
    class CKSimMotor : public frc::Sendable
    {
    public:
        CKSimMotor(int id);
        float GetMotorValue();
        void SetMotorValue(float value);
        float GetEncoderValue();

    protected:
        void InitSendable(frc::SendableBuilder &builder) override;

    private:
        int id;
        bool validateId(int id);
    };
} // namespace team195