package org.phash;

public class AudioHash {

    public native static float[] readAudio(String filename, int sr, float nbsecs, AudioMetaData mdataObj);

    public native static AudioHashObject audioHash(float[] buf, int P, int sr);

    static {
	System.loadLibrary("pHashAudio-jni");
    }

}
