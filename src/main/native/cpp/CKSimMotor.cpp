#include "team195/CKSimMotor.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"

#include <iostream>

#include "team195/CKSim.h"

namespace team195
{
    CKSimMotor::CKSimMotor(int id) : id(id)
    {
        if (!validateId(id))
        {
            std::cout << "Invalid motor ID: " << id << std::endl;
            id = -1;
            return;
        }
        if (!CKSim::GetInstance().registerMotor(id))
        {
            std::cout << "Failed to add motor ID: " << id << "; ID Already in use!" << std::endl;
            id = -1;
            return;
        }
        frc::SendableRegistry::GetInstance().AddLW(this, "CKSimMotor", id);
    }

    float CKSimMotor::GetMotorValue()
    {
        return CKSim::GetInstance().getMotorValue(id);
    }

    void CKSimMotor::SetMotorValue(float value)
    {
        CKSim::GetInstance().setMotorValue(id, value);
    }

    float CKSimMotor::GetEncoderValue()
    {
        return CKSim::GetInstance().getEncoderValue(id);
    }

    bool CKSimMotor::validateId(int id)
    {
        return (id >= 0 && id < CKSim::MAX_NUM_MOTORS);
    }

    void CKSimMotor::InitSendable(frc::SendableBuilder &builder)
    {
        builder.SetSmartDashboardType("CKSimMotor");
        // builder.SetActuator(true);
        // builder.SetSafeState([=]() { SetDisabled(); });
        builder.AddDoubleProperty(
            "Value", [=]() { return (double)GetMotorValue(); },
            [=](double value) { SetMotorValue((float)value); });
    }
} // namespace team195