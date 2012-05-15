/*

 pHash, the open source perceptual hash library
 Copyright (C) 2010 Aetilius, Inc.
 All rights reserved.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 Evan Klinger - eklinger@phash.org
 David Starkweather - dstarkweather@phash.org

 */

#include <jni.h>
#include "AudioScout-jni.h"

#include "audiophash.h"

jfieldID audioHash_hash = NULL;

jfieldID hash_filename = NULL;

jclass audioClass = NULL;

jmethodID audioCtor = NULL;

typedef enum ph_jni_hash_types {
	AUDIO_HASH, 
} jniHashType;

typedef struct ph_jni_hash_classes {
	jclass *cl;
	jniHashType kind;
	jmethodID *ctor;
	jfieldID *hashID;
} jniHashes;

const static jniHashes hashes[] = { 

{ &audioClass, 
		AUDIO_HASH, &audioCtor, &audioHash_hash }, };

JNIEXPORT void JNICALL Java_org_phash_pHash_pHashInit
(JNIEnv *e, jclass cl)
{

	audioClass = (jclass)e->NewGlobalRef(e->FindClass("org/phash/AudioHash"));

	audioHash_hash = e->GetFieldID(audioClass, "hash", "[I");

	hash_filename = e->GetFieldID(e->FindClass("org/phash/Hash"), "filename", "Ljava/lang/String;");

	audioCtor = e->GetMethodID(audioClass, "<init>", "()V");

}
JNIEXPORT void JNICALL Java_org_phash_pHash_cleanup
(JNIEnv *e, jclass cl)
{
	e->DeleteGlobalRef(audioClass);

}
JNIEXPORT jdouble JNICALL Java_org_phash_pHash_audioDistance
(JNIEnv *e, jclass cl, jobject audioHash1, jobject audioHash2)
{

	if(audioHash1 == NULL || audioHash2 == NULL)
	{
		return (jdouble)-1.0;
	}
	const float threshold = 0.30;
	const int block_size = 256;
	int Nc;
	jdouble maxC = 0.0;
	double *pC;
	jint hash1_len, hash2_len;
	jintArray hash1, hash2;
	hash1 = (jintArray)e->GetObjectField(audioHash1, audioHash_hash);
	hash2 = (jintArray)e->GetObjectField(audioHash2, audioHash_hash);
	hash1_len = e->GetArrayLength(hash1);
	hash2_len = e->GetArrayLength(hash2);
	if(hash1_len <= 0 || hash2_len <= 0)
	{
		return (jdouble)-1.0;
	}
	uint32_t *hash1_n, *hash2_n;

	hash1_n = (uint32_t*)e->GetIntArrayElements(hash1, 0);
	hash2_n = (uint32_t*)e->GetIntArrayElements(hash2, 0);
	pC = ph_audio_distance_ber(hash1_n, hash1_len, hash2_n, hash2_len, threshold, block_size, Nc);
	e->ReleaseIntArrayElements(hash1, (jint*)hash1_n, 0);
	e->ReleaseIntArrayElements(hash2, (jint*)hash2_n, 0);
	maxC = 0.0;
	for (int j=0;j<Nc;j++) {
		if (pC[j] > maxC) {
			maxC = pC[j];
		}
	}
	delete[] pC;
	return maxC;

}
JNIEXPORT jobject JNICALL Java_org_phash_pHash_audioHash
(JNIEnv *e, jclass cl, jstring f)
{
	jintArray ret = NULL;
	const int sr = 8000;
	const int channels = 1;
	int N;
	int nbframes;
	float *buf = NULL;
	unsigned int *hash = NULL;
	const char *file = e->GetStringUTFChars(f,0);
	buf = ph_readaudio(file,sr,channels,NULL,N);
	if(!buf)
	{
		e->ReleaseStringUTFChars(f,file);
		return NULL;
	}
	hash = ph_audiohash(buf,N,sr,nbframes);
	if(!hash || nbframes <= 0)
	{
		free(buf);
		return NULL;
	}
	free(buf);

	jobject audioHash = e->NewObject(audioClass, audioCtor);
	e->SetObjectField(audioHash, hash_filename, f);
	e->ReleaseStringUTFChars(f,file);

	jintArray hashVals = e->NewIntArray(nbframes);

	e->SetIntArrayRegion(hashVals, 0, nbframes, (jint *)hash);
	e->SetObjectField(audioHash, audioHash_hash, hashVals);
	free(hash);

	return audioHash;

}
