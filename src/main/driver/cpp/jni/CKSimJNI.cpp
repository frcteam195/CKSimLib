#include "com_team195_jni_CKSimJNI.h"

// We have a lot of JNI utilities here
// If you need them. For things like strings and
// arrays
#include <wpi/jni_util.h>
#include "team195/CKSimDriver.h"

extern "C" {
/*
 * Class:     com_team195_jni_CKSimJNI
 * Method:    Run
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_com_team195_jni_CKSimJNI_Run
  (JNIEnv* env, jclass, jstring fileName)
{
    c_CKSimDriver();
	return 0;
}
}  // extern "C"
