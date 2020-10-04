#include "com_team195_jni_CKSimJNI.h"

// We have a lot of JNI utilities here
// If you need them. For things like strings and
// arrays
#include <wpi/jni_util.h>
#include "team195/CKSimDriver.h"

extern "C"
{
    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    SetIP
    * Signature: ([B)V
    */
    JNIEXPORT void JNICALL Java_com_team195_jni_CKSimJNI_SetIP(JNIEnv *env, jclass, jbyteArray ipArr)
    {
        jboolean isCopy;
        jsize arrLen = env->GetArrayLength(ipArr);
        jbyte *b = env->GetByteArrayElements(ipArr, &isCopy);
        char tmpIPArr[64];
        for (int i = 0; i < (arrLen < 64 ? arrLen : 64); i++)
        {
            tmpIPArr[i] = b[i];
        }
        env->ReleaseByteArrayElements(ipArr, b, JNI_ABORT);
        tmpIPArr[arrLen] = 0; //Ensure string for java is null terminated
        c_CKSimSetIP(tmpIPArr);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    GetMotorValue
    * Signature: (I)F
    */
    JNIEXPORT jfloat JNICALL Java_com_team195_jni_CKSimJNI_GetMotorValue(JNIEnv *env, jclass, jint id)
    {
        return c_GetMotor(id);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    SetMotorValue
    * Signature: (IF)V
    */
    JNIEXPORT void JNICALL Java_com_team195_jni_CKSimJNI_SetMotorValue(JNIEnv *env, jclass, jint id, jfloat val)
    {
        c_SetMotor(id, val);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    GetEncoderValue
    * Signature: (I)F
    */
    JNIEXPORT jfloat JNICALL Java_com_team195_jni_CKSimJNI_GetEncoderValue(JNIEnv *env, jclass, jint id)
    {
        return c_GetEncoder(id);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    SetEncoderValue
    * Signature: (IF)V
    */
    JNIEXPORT void JNICALL Java_com_team195_jni_CKSimJNI_SetEncoderValue(JNIEnv *env, jclass, jint id, jfloat val)
    {
        c_SetEncoder(id, val);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    GetAccelerometerValue
    * Signature: (I)F
    */
    JNIEXPORT jfloat JNICALL Java_com_team195_jni_CKSimJNI_GetAccelerometerValue(JNIEnv *env, jclass, jint id)
    {
        return c_GetAccelerometer(id);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    SetAccelerometerValue
    * Signature: (IF)V
    */
    JNIEXPORT void JNICALL Java_com_team195_jni_CKSimJNI_SetAccelerometerValue(JNIEnv *env, jclass, jint id, jfloat val)
    {
        c_SetAccelerometer(id, val);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    GetGyroValue
    * Signature: (I)F
    */
    JNIEXPORT jfloat JNICALL Java_com_team195_jni_CKSimJNI_GetGyroValue(JNIEnv *env, jclass, jint id)
    {
        return c_GetGyro(id);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    SetGyroValue
    * Signature: (IF)V
    */
    JNIEXPORT void JNICALL Java_com_team195_jni_CKSimJNI_SetGyroValue(JNIEnv *env, jclass, jint id, jfloat val)
    {
        c_SetGyro(id, val);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    GetAdvObjValue
    * Signature: (I)F
    */
    JNIEXPORT jfloat JNICALL Java_com_team195_jni_CKSimJNI_GetAdvObjValue(JNIEnv *, jclass, jint id)
    {
        return c_GetAdvObj(id);
    }

    /*
    * Class:     com_team195_jni_CKSimJNI
    * Method:    SetAdvObjValue
    * Signature: (IF)V
    */
    JNIEXPORT void JNICALL Java_com_team195_jni_CKSimJNI_SetAdvObjValue(JNIEnv *, jclass, jint id, jfloat val)
    {
        c_SetAdvObj(id, val);
    }
} // extern "C"
