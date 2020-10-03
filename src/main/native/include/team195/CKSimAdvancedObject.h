#pragma once

namespace team195
{
    class CKSimAdvancedObject
    {
    public:
        CKSimAdvancedObject(int id);
        float GetValue();

    private:
        int id;
        bool validateId(int id);
    };
} // namespace team195