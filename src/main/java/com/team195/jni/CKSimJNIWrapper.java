package com.team195.jni;

import java.io.IOException;

import edu.wpi.first.wpiutil.RuntimeLoader;

public class CKSimJNIWrapper {
    static boolean libraryLoaded = false;
    static RuntimeLoader<CKSimJNIWrapper> loader = null;

    static {
        if (!libraryLoaded) {
            try {
                loader = new RuntimeLoader<>("CKSimDriver", RuntimeLoader.getDefaultExtractionRoot(),
                        CKSimJNIWrapper.class);
                loader.loadLibrary();
            } catch (IOException ex) {
                ex.printStackTrace();
                System.exit(1);
            }
            libraryLoaded = true;
        }
    }
}
