#pragma once

#include <frc/smartdashboard/Sendable.h>
#include "frc/smartdashboard/SendableBuilder.h"

namespace team195
{
    class CKSimAdvancedObject : public frc::Sendable
    {
    public:
        CKSimAdvancedObject(int id);
        float GetValue();

    protected:
        void InitSendable(frc::SendableBuilder &builder) override;

    private:
        int id;
        bool validateId(int id);
    };
} // namespace team195