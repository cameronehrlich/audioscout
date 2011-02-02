#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "serialize.h"


int main(int argc, char **argv){

    uint32_t s = 0x01020304;
    uint32_t t, u;

    t = hosttonet32(s);
    u = nettohost32(t);

    printf("s = %x, t = %x, u = %x \n", s, t, u);
    
    uint32_t nb = 2781;
    uint32_t nb2 = hosttonet32(nb);
    uint32_t nb3 = nettohost32(nb2);

    printf("nb = %u => %u => %u\n", nb, nb2, nb3);
    printf("nb = %x => %x => %x\n", nb, nb2, nb3);
    uint32_t nbframes = 2573;
    uint32_t nbframes2 = 2781;
    printf(" 2573 = %x\n", nbframes);
    printf(" 2781 = %x\n", nbframes2);


    return 0;
}
