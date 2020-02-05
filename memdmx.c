#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>

typedef struct _iqs_ {
    short i;
    short q;
} iqs;

void demux(iqs *src, iqs *dst0, iqs *dst1, int len) {
    for(int i = 0, j=0;i<len;i+=2,j++) {
        dst0[j] = src[i];
        dst1[j] = src[i+1];
    }
}

inline unsigned long get_mks() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec*1000000 + tv.tv_usec;
}

int main() {
    int len;
    if (!fscanf(stdin, "%i", &len) ) {
        return 0;
    };
    iqs *src = malloc(len*sizeof(iqs));
    iqs *dst0 = malloc(len/2*sizeof(iqs));
    iqs *dst1 = malloc(len/2*sizeof(iqs));
    srand(len);
    for(int i = 0;i<len;i++) {
        src[i].i = rand() % 2 + 1;
        src[i].q = rand() % 2 + 1;
    }
    unsigned long start_mks = get_mks();
    demux(src, dst0, dst1, len);
    unsigned long end_mks = get_mks();
    unsigned long mks = end_mks-start_mks;

    double MBytes = (((double) len*2*sizeof(iqs)) / (1024*1024.0f));
    double secs = mks / 1000000.0f;
    printf("DemuxTime %lu size %f speed %f Mb/s\n", mks, MBytes, MBytes/secs);

    int sum = 0;
    for(int i = 0;i<len/2;i++) {
        sum += dst0[i].i;
        sum += dst1[i].q;
        sum -= src[i].q;
    }
    free(dst0);
    free(dst1);
    free(src);
    printf("Random sum %d\n", sum);

    return 0;
}
