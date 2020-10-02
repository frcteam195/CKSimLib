#pragma once

template <typename T>
class CKSingleton
{
    friend T;

public:
    static T &GetInstance()
    {
        static T inst;
        return inst;
    }

private:
    CKSingleton() = default;
    ~CKSingleton() = default;
    CKSingleton(const CKSingleton &) = delete;
    CKSingleton &operator=(const CKSingleton &) = delete;
};