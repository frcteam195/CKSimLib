#include "team195/CKSimAdvancedObject.h"
#include "frc/smartdashboard/SendableBuilder.h"
#include "frc/smartdashboard/SendableRegistry.h"
#include <iostream>

#include "team195/CKSim.h"

namespace team195
{
    CKSimAdvancedObject::CKSimAdvancedObject(int id) : id(id)
    {
        if (!validateId(id))
        {
            std::cout << "Invalid advanced object ID: " << id << std::endl;
            id = -1;
            return;
        }
        if (!CKSim::GetInstance().registerAdvObj(id))
        {
            std::cout << "Failed to add advanced object ID: " << id << "; ID Already in use!" << std::endl;
            id = -1;
            return;
        }
        frc::SendableRegistry::GetInstance().AddLW(this, "CKSimAdvanced", id);
    }

    float CKSimAdvancedObject::GetValue()
    {
        return CKSim::GetInstance().getAdvObjValue(id);
    }

    bool CKSimAdvancedObject::validateId(int id)
    {
        return (id >= 0 && id < CKSim::MAX_NUM_ADVOBJ);
    }

    void CKSimAdvancedObject::InitSendable(frc::SendableBuilder &builder)
    {
        builder.SetSmartDashboardType("CKSimAdvanced");
        // builder.SetActuator(true);
        // builder.SetSafeState([=]() { SetDisabled(); });
        builder.AddDoubleProperty(
            "Value", [=]() { return (double)GetValue(); }, [=](double value) { NULL; });
    }
} // namespace team195