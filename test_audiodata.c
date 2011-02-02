#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "audiodata.h"


static const char *testfile = "./testdir/sample.mp3";


int main(int argc, char **argv){


  float *sigbuf = (float*)malloc(1<<28);
  const unsigned int buflen = (1<<28)/sizeof(float);


  AudioMetaData mdata;
  float *buf = NULL;
  unsigned int len;


  float nbsecs = 30.0f; /* full audio */ 
  int sr = 5512;
  len = buflen;

  printf("testing  %s @ sr %d, for %f seconds...\n", testfile, sr, nbsecs);
  buf = readaudio(testfile, sr, sigbuf, &len, nbsecs, &mdata);
  assert(buf);
  assert(len == 165359 || len == 165360);
  assert(mdata.title2);
  assert(strchr(mdata.title2, 'B') < mdata.title2 + strlen(mdata.title2)) ;
  assert(mdata.composer);
  assert(strchr(mdata.composer, 'T') < mdata.composer + strlen(mdata.composer));
  printf("ok\n");

  free_mdata(&mdata);
  if (buf != sigbuf) free(buf);


  buf = NULL;
  sr = 8000;
  nbsecs = 60.0f;
  len = buflen;

  printf("testing %s @ sr = %d, for %f seconds...\n", testfile, sr, nbsecs);
  buf = readaudio(testfile, sr, sigbuf, &len, nbsecs, &mdata);
  assert(buf);
  assert(len == 479999 || len == 480000);
  printf("ok\n");

  free_mdata(&mdata);
  if (buf != sigbuf) free(buf);


  buf = NULL;
  sr = 11025;
  nbsecs = 0.0f;
  len = buflen;

  printf("testing %s @ sr = %d for %f seconds...\n", testfile, sr, nbsecs);
  buf = readaudio(testfile, sr, sigbuf, &len, nbsecs, &mdata);
  assert(buf);
  assert(len == 671771);
  printf("ok\n");
  
  free_mdata(&mdata);
  if (buf != sigbuf) free(buf);

  printf("done\n");
  free(sigbuf);

  return 0;
}
