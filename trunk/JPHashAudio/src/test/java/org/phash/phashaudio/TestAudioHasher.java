package org.phash.phashaudio;

import java.util.Arrays;
import junit.framework.TestCase;
import org.junit.Before;
import org.junit.Test;
import org.junit.After;
import org.junit.BeforeClass;
import org.junit.AfterClass;

public class TestAudioHasher {

    protected final int sr = 6000;

    protected final int p = 10;

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
	
	for (int i=0;i<hash.hasharray.length;i++){
	    int nbbits = Integer.bitCount(hash.toggles[i]);
	    assert(nbbits == p);
	}
	assert(hash.hasharray != null);
	assert(hash.coeffs != null);
	assert(hash.toggles != null);

	assert(hash.hasharray.length == 60);
	assert(hash.toggles.length == 60);
	System.out.println("Done.");
    }

    @Test public void testDistanceBer(){
	System.out.println("test distance ber() ...");

	AudioHashInfo hash = hasher.calc(buf, 0);
	
	float[] subbuf = Arrays.copyOfRange(buf, 401, 4000);
	AudioHashInfo subhash = hasher.calc(subbuf, 4);

	AudioHashDistance dist = hasher.audiohash_distance_ber(subhash, hash);
	assert(dist.pos == 6);
	assert(dist.cs >= 0.99);
	System.out.printf("pos = %d, cs = %f\n", dist.pos, dist.cs);
	System.out.printf("Done.\n");
    }
}