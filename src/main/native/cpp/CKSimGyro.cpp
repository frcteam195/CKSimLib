#include "team195/CKSimGyro.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"
#include <iostream>

#include "team195/CKSim.h"

namespace team195
{
    CKSimGyro::CKSimGyro(int id, int numAxes) : id(id), numAxes(numAxes)
    {
        if (!validateId(id))
        {
            std::cout << "Invalid gyro ID: " << id << std::endl;
            id = -1;
            return;
        }
        if (!validateId(id + numAxes - 1))
        {
            std::cout << "Invalid last axis gyro ID: " << (id + numAxes - 1) << std::endl;
            id = -1;
            return;
        }
        for (int i = id; i < id + numAxes; i++)
        {
            if (!CKSim::GetInstance().registerGyro(i))
            {
                std::cout << "Failed to add gyro ID: " << i << "; ID Already in use! Please make sure ids for additional axes are not in use. Each axis consumes the next consecutive id." << std::endl;
                id = -1;
                return;
            }
        }
        frc::SendableRegistry::GetInstance().AddLW(this, "CKSimGyro", id);
    }

    float CKSimGyro::GetValue(int axis)
    {
        if (axis >= 0 && axis < numAxes && validateId(id + axis) && CKSim::GetInstance().containsGyro(id + axis))
        {
            return CKSim::GetInstance().getGyroValue(id + axis);
        }
        else
        {
            std::cout << "Invalid axis: " << axis << std::endl;
        }
        return 0;
    }

    bool CKSimGyro::validateId(int id)
    {
        return (id >= 0 && id < CKSim::MAX_NUM_GYRO);
    }

    void CKSimGyro::InitSendable(frc::SendableBuilder &builder)
    {
        builder.SetSmartDashboardType("CKSimGyro");
        // builder.SetActuator(true);
        // builder.SetSafeState([=]() { SetDisabled(); });
        for (int i = 0; i < numAxes; i++)
        {
            const int idx = i;
            builder.AddDoubleProperty(
                "Axis " + std::to_string(idx), [=]() { return (double)GetValue(idx); }, [=](double value) { NULL; });
        }
    }
} // namespace team195