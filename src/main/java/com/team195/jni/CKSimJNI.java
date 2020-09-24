package com.team195.jni;

public class CKSimJNI extends CKSimJNIWrapper {

  /**
   * Begin the software.
   * 
   * @param fileName The filename of the log.
   * 
   * @return Success
   */
  public static native int Run(String fileName);
}