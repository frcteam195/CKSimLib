#pragma once

namespace team195
{
    class CKSimAccelerometer
    {
    public:
        CKSimAccelerometer(int id, int numAxes);
        float GetValue(int axis);

    private:
        int id;
        int numAxes;
        bool validateId(int id);
    };
} // namespace team195