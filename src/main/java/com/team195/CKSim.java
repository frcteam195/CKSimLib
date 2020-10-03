package com.team195;

import com.team195.jni.CKSimJNI;
import java.util.HashSet;

public class CKSim {
    private static CKSimJNI m_jniInstance = new CKSimJNI();
    private static CKSim mInstance = new CKSim();

    public static final int MAX_NUM_MOTORS = 64;
    public static final int MAX_NUM_ACCEL = 64;
    public static final int MAX_NUM_GYRO = 64;
    public static final int MAX_NUM_ADVOBJ = 64;

    private final HashSet<Integer> motorReg;
    private final HashSet<Integer> accelReg;
    private final HashSet<Integer> gyroReg;
    private final HashSet<Integer> advObjReg;

    protected static CKSim getInstance() {
        return mInstance;
    }

    private CKSim() {
        motorReg = new HashSet<>();
        accelReg = new HashSet<>();
        gyroReg = new HashSet<>();
        advObjReg = new HashSet<>();
    }

    public boolean registerMotor(int id) {
        return motorReg.add(id);
    }

    public boolean containsMotor(int id) {
        return motorReg.contains(id);
    }

    public boolean registerAccelerometer(int id) {
        return accelReg.add(id);
    }

    public boolean containsAccelerometer(int id) {
        return accelReg.contains(id);
    }

    public boolean registerGyro(int id) {
        return gyroReg.add(id);
    }

    public boolean containsGyro(int id) {
        return gyroReg.contains(id);
    }

    public boolean registerAdvObj(int id) {
        return advObjReg.add(id);
    }

    public boolean containsAdvObj(int id) {
        return advObjReg.contains(id);
    }

    public float getMotorValue(int id) {
        return CKSimJNI.GetMotorValue(id);
    }

    public void setMotorValue(int id, float val) {
        CKSimJNI.SetMotorValue(id, val);
    }

    public float getEncoderValue(int id) {
        return CKSimJNI.GetEncoderValue(id);
    }

    public void setEncoderValue(int id, float val) {
        CKSimJNI.SetEncoderValue(id, val);
    }

    public float getAccelerometerValue(int id) {
        return CKSimJNI.GetAccelerometerValue(id);
    }

    public void setAccelerometerValue(int id, float val) {
        CKSimJNI.SetAccelerometerValue(id, val);
    }

    public float getGyroValue(int id) {
        return CKSimJNI.GetGyroValue(id);
    }

    public void setGyroValue(int id, float val) {
        CKSimJNI.SetGyroValue(id, val);
    }

    public float getAdvObjValue(int id) {
        return CKSimJNI.GetAdvObjValue(id);
    }

    public void setAdvObjValue(int id, float val) {
        CKSimJNI.SetAdvObjValue(id, val);
    }
}
