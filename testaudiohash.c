#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#ifdef _WIN32
#define BUILD_EXE
#endif

#include "phash_audio.h"
#include "audiodata.h"

int main (int argc, char **argv){
  const int sr = 6000;
  const float nbsecs = 0.0f;

  if (argc < 2){
    printf("not enough input args\n");
    return -1;
  }

  const char *dirname = argv[1];
  printf("dirname: %s\n", dirname);


  unsigned int nbfiles;
  char **files = readfilenames(dirname, &nbfiles);
    
  printf("nbfiles %u\n", nbfiles);

  unsigned int buflen = 0;

  AudioHashStInfo hash_st;
  hash_st.sr = 0;

  clock_t total = 0;
  unsigned int i;
  int err;
  for (i=0;i<nbfiles;i++){
    printf("file[%u]: %s\n", i, files[i]);
    
    float *buf = readaudio(files[i], sr, NULL, &buflen, nbsecs, NULL, &err);
    if (buf == NULL){
      printf("unable to read audio: error number %d\n",err);
      break;
    }
    printf("buf %p, len %u\n", buf, buflen);

    clock_t start = clock();
    uint32_t *hash = NULL;
    unsigned int nbframes;
    if (audiohash(buf, &hash, NULL, NULL, NULL, &nbframes, NULL, NULL, buflen, 0, sr,&hash_st) < 0){
      printf("unable to get hash \n");
      break;
    }
    clock_t end = clock();
    clock_t dur = end - start;
    total += dur;
    double secs = (double)dur/(double)CLOCKS_PER_SEC;
    printf("hash %p, nbframes %u, %f seconds\n", hash, nbframes,secs);

    ph_free(hash);
    ph_hashst_free(&hash_st);
    ph_free(buf);
  }

  double average_seconds = (double)total/(double)nbfiles/(double)CLOCKS_PER_SEC;
  printf("ave hash time %f seconds\n", average_seconds);
  for (i = 0;i<nbfiles;i++){
    free(files[i]);
  }
  free(files);

  return 0;
}
