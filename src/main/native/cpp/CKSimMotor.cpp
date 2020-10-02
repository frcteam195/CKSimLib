#include "team195/CKSimMotor.h"

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
} // namespace team195