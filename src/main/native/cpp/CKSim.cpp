#include "team195/CKSim.h"

#include "team195/CKSimDriver.h"
#include <unordered_set>

namespace team195
{

    CKSim::CKSim()
    {
        if (c_CKSimDriver() < 0)
        {
            throw std::exception("CKSim Driver Init failed!");
        }
    }

    CKSim::~CKSim()
    {
        c_CKSimDealloc();
    }

    void CKSim::SetIP(std::string ip)
    {
        c_CKSimSetIP(ip.c_str());
    }

    static std::unordered_set<int> motorReg;
    bool CKSim::registerMotor(int id)
    {
        return motorReg.insert(id).second; //Return success/failure bool from pair
    }
    bool CKSim::containsMotor(int id)
    {
        return motorReg.find(id) != motorReg.end();
    }

    static std::unordered_set<int> accelReg;
    bool CKSim::registerAccelerometer(int id)
    {
        return accelReg.insert(id).second;
    }
    bool CKSim::containsAccelerometer(int id)
    {
        return accelReg.find(id) != accelReg.end();
    }

    static std::unordered_set<int> gyroReg;
    bool CKSim::registerGyro(int id)
    {
        return gyroReg.insert(id).second;
    }
    bool CKSim::containsGyro(int id)
    {
        return gyroReg.find(id) != gyroReg.end();
    }

    static std::unordered_set<int> advObjReg;
    bool CKSim::registerAdvObj(int id)
    {
        return advObjReg.insert(id).second;
    }
    bool CKSim::containsAdvObj(int id)
    {
        return advObjReg.find(id) != advObjReg.end();
    }

    float CKSim::getMotorValue(int id)
    {
        return c_GetMotor(id);
    }
    void CKSim::setMotorValue(int id, float val)
    {
        c_SetMotor(id, val);
    }

    float CKSim::getEncoderValue(int id)
    {
        return c_GetEncoder(id);
    }
    void CKSim::setEncoderValue(int id, float val)
    {
        c_SetEncoder(id, val);
    }

    float CKSim::getAccelerometerValue(int id)
    {
        return c_GetAccelerometer(id);
    }
    void CKSim::setAccelerometerValue(int id, float val)
    {
        c_SetAccelerometer(id, val);
    }

    float CKSim::getGyroValue(int id)
    {
        return c_GetGyro(id);
    }
    void CKSim::setGyroValue(int id, float val)
    {
        c_SetGyro(id, val);
    }

    float CKSim::getAdvObjValue(int id)
    {
        return c_GetAdvObj(id);
    }
    void CKSim::setAdvObjValue(int id, float val)
    {
        c_SetAdvObj(id, val);
    }
} // namespace team195