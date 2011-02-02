#include <stdlib.h>
#include <stdio.h>
#include <phash_audio.h>

int main(int argc, char **argv){
    if (argc < 3){
	printf("not enough input args\n");
	printf("usage: %s dstindexfile srcindexfile\n", argv[0]);
	exit(1);
    }
    const char *dstfile = argv[1];
    const char *srcfile = argv[2];

    printf("merging %s into %s\n", srcfile, dstfile);
    int err = merge_audioindex(dstfile, srcfile);
    if (err < 0){
	printf("could not merge successfully: %d\n", err);
    } else {
	printf("merged %d \n", err);
    }
    printf("done\n");

    return 0;
}
