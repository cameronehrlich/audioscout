package org.phash;
import java.io.*;

public class pHash
{

	public native static AudioHash audioHash(String file);
	public native static double audioDistance(AudioHash hash1, AudioHash hash2);
	private native static void pHashInit();
	private native static void cleanup();


	static {
        System.loadLibrary("AudioScout-jni");
		pHashInit();
	}

	public static void main(String args[])
	{
        int i = 0;
        if(args[i].equals("-a"))
        {
            AudioHash audioHash1 = audioHash(args[1]);
            AudioHash audioHash2 = audioHash(args[2]);
            System.out.println("cs = " + audioDistance(audioHash1,audioHash2));
        }

        pHash.cleanup();
	}
}
