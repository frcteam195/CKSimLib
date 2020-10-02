package com.team195.jni;

public class CKSimJNI extends CKSimJNIWrapper {
    public static native float GetMotorValue(int id);

    public static native void SetMotorValue(int id, float val);

    public static native float GetEncoderValue(int id);

    public static native void SetEncoderValue(int id, float val);

    public static native float GetAccelerometerValue(int id);

    public static native void SetAccelerometerValue(int id, float val);

    public static native float GetGyroValue(int id);

    public static native void SetGyroValue(int id, float val);
}