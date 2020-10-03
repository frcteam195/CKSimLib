#include "team195/CKSimAdvancedObject.h"

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
    }

    float CKSimAdvancedObject::GetValue()
    {
        return CKSim::GetInstance().getAdvObjValue(id);
    }

    bool CKSimAdvancedObject::validateId(int id)
    {
        return (id >= 0 && id < CKSim::MAX_NUM_ADVOBJ);
    }
} // namespace team195