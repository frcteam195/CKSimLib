#pragma once

#include <frc/smartdashboard/Sendable.h>
#include "frc/smartdashboard/SendableBuilder.h"

namespace team195
{
    class CKSimGyro : public frc::Sendable
    {
    public:
        CKSimGyro(int id, int numAxes);
        float GetValue(int axis);

    protected:
        void InitSendable(frc::SendableBuilder &builder) override;

    private:
        int id;
        int numAxes;
        bool validateId(int id);
    };
} // namespace team195