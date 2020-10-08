#include <jni.h>

#include "team195/CKSimDriver.h"

extern "C"
{

    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
    {
        // Check to ensure the JNI version is valid

        JNIEnv *env;
        if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
        {
            return JNI_ERR;
        }

        // In here is also where you store things like class references
        // if they are ever needed
        if (c_CKSimDriver() < 0)
        {
            return JNI_ERR;
        }

        return JNI_VERSION_1_6;
    }

    JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
    {
        c_CKSimDealloc();
    }
}
