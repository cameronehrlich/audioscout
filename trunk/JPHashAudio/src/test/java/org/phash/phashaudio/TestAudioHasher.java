package org.phash.phashaudio;

import java.io.FileInputStream;
import java.util.Scanner;
import java.util.Arrays;
import junit.framework.TestCase;
import org.junit.Before;
import org.junit.Test;
import org.junit.After;
import org.junit.BeforeClass;
import org.junit.AfterClass;

public class TestAudioHasher {

    protected final String testfile = "./sample.txt";

    protected final int sr = 6000;

    protected final int p = 12;

    protected final int buflen = 6000;

    protected AudioHasher hasher;

    protected float[] buf;

    @Before public void testCreateAudioHasher(){
	System.out.println("create audiohasher()");
	this.buf = new float[buflen];
	for (int i=0;i<buflen;i++){
	    buf[i] = (float)Math.random();
	}
	this.hasher = new AudioHasher(sr);
	assert(hasher != null);
	System.out.println("Done.");
    }

    @Test public void testCalc(){
	System.out.println("test calc()...");
	AudioHashInfo hash = hasher.calc(buf, p);
	System.out.println("Done.");
    }

    @Test public void testCalc2(){
	try {
	    System.out.println("test calc2()");
	    
	    Scanner scanner = new Scanner(new FileInputStream(testfile));

	    int nbsamples = (int)scanner.nextFloat();

	    buf = new float[nbsamples];
	    int count = 0;
	    while (scanner.hasNextFloat()){
		buf[count++] = scanner.nextFloat();
	    }
	    assert(count == nbsamples);
	    scanner.close();

	    AudioHashInfo hash = hasher.calc(buf, p);

	    assert(hash.hasharray[0] == 0xcb0f9ef0);
	    assert(hash.hasharray[1] == 0xcb0fbaf0);
	    assert(hash.hasharray[2] == 0xcb0eb870);
	    assert(hash.hasharray[3] == 0xcb0ef870);
	    assert(hash.hasharray[30] == 0xf0b86b00);
	    assert(hash.hasharray[31] == 0xf0b86901);
	    assert(hash.hasharray[32] == 0xf0b8780b);
	    
	    assert(hash.toggles[0] == 0x6889fc00);
	    assert(hash.toggles[1] == 0x6889fc00);
	    assert(hash.toggles[2] == 0x6889db00);
	    assert(hash.toggles[3] == 0x3889d980);
	    assert(hash.toggles[30] == 0x21c8fe00);
	    assert(hash.toggles[31] == 0x9c8f700);
	    assert(hash.toggles[32] == 0x8c8ff00);

	    assert(hash.hasharray != null);
	    assert(hash.coeffs != null);
	    assert(hash.toggles != null);
	    assert(hash.toggles.length == 33);
	    assert(hash.coeffs[0].length == 33);
	    assert(hash.coeffs.length == 35);
	} catch (Exception ex){
	    assert(false);
	}
    }
    
    @After public void testDistanceBer(){
	System.out.println("test distance ber() ...");
	AudioHashInfo hash = hasher.calc(buf, 0);
	float[] subbuf = Arrays.copyOfRange(buf, 401, 4000);
	AudioHashInfo subhash = hasher.calc(subbuf, 4);
	AudioHashDistance dist = hasher.audiohash_distance_ber(subhash, hash);
	assert(dist.pos == 6);
	assert(dist.cs >= 0.98 && dist.cs <= 1.00);
    }
}