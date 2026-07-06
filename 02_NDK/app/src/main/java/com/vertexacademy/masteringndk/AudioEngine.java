package com.vertexacademy.masteringndk;

public class AudioEngine {
    static {
        System.loadLibrary("native-lib");
    }

    // Déclaration des fonctions natives C++
    public static native boolean initOpenAL();
    public static native void playTestSound();
    public static native void shutdownOpenAL();
    public static native void renderFont(Object assetManager);
}