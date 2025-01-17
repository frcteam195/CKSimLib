#include "team195/CKSimAccelerometer.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"
#include <iostream>

#include "team195/CKSim.h"

namespace team195
{
    CKSimAccelerometer::CKSimAccelerometer(int id, int numAxes) : id(id), numAxes(numAxes)
    {
        if (!validateId(id))
        {
            std::cout << "Invalid accelerometer ID: " << id << std::endl;
            id = -1;
            return;
        }
        if (!validateId(id + numAxes - 1))
        {
            std::cout << "Invalid last axis accelerometer ID: " << (id + numAxes - 1) << std::endl;
            id = -1;
            return;
        }
        for (int i = id; i < id + numAxes; i++)
        {
            if (!CKSim::GetInstance().registerAccelerometer(i))
            {
                std::cout << "Failed to add accelerometer ID: " << i << "; ID Already in use! Please make sure ids for additional axes are not in use. Each axis consumes the next consecutive id." << std::endl;
                id = -1;
                return;
            }
        }
        frc::SendableRegistry::GetInstance().AddLW(this, "CKSimAccelerometer", id);
    }

    float CKSimAccelerometer::GetValue(int axis)
    {
        if (axis >= 0 && axis < numAxes && validateId(id + axis) && CKSim::GetInstance().containsAccelerometer(id + axis))
        {
            return CKSim::GetInstance().getAccelerometerValue(id + axis);
        }
        else
        {
            std::cout << "Invalid axis: " << axis << std::endl;
        }
        return 0;
    }

    bool CKSimAccelerometer::validateId(int id)
    {
        return (id >= 0 && id < CKSim::MAX_NUM_ACCEL);
    }

    void CKSimAccelerometer::InitSendable(frc::SendableBuilder &builder)
    {
        builder.SetSmartDashboardType("CKSimAccelerometer");
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