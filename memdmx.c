#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#define TEST_TYPE int64_t

inline unsigned long get_mks() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void memcpy_test(TEST_TYPE* dst, const TEST_TYPE* src, int len) {
    for (int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}

TEST_TYPE sumsum(const TEST_TYPE *src, int len) {
    TEST_TYPE result = 0;
    for(int i=0;i<len;i++) {
        result += src[i];
    }
    return result;
}

void memdmx(const TEST_TYPE *src, TEST_TYPE *dst0, TEST_TYPE *dst1, int samples) {
    for(int i=0, j=0;i<samples;i+=2, j++) {
        dst0[j] = src[i];
        dst1[j] = src[i+1];
    }
}

int main() {
    int samples = 128 * 1024 * 1024;
    printf("Allocating %lu bytes ", samples * sizeof(TEST_TYPE));
    int alloc_len8 = samples * sizeof(TEST_TYPE);

    TEST_TYPE* src = malloc(alloc_len8);
    TEST_TYPE* dst = malloc(alloc_len8);
    TEST_TYPE* dst1 = malloc(alloc_len8);

    printf("src %p dst %p dst1 %p\n", src, dst, dst1);

    double MBytes = (((double) alloc_len8) / (1024 * 1024.0));

    srand(samples);
    for (int i = 0; i < samples; i++) {
        src[i] = rand();
        dst[i] = rand();
        dst1[i] = rand();
    }
    printf("Sleeping...\n");
    usleep(1000000);

    printf("Running!\n");
    unsigned long start_mks = get_mks();
    memcpy(dst, src, alloc_len8);
    unsigned long end_mks = get_mks();

    unsigned long mks = end_mks - start_mks;
    double secs = mks / 1000000.0;

    printf("Native   memcpy: time %8lu mks size %4.2f MBytes speed %6.2f MB/s\n", mks, MBytes, MBytes / secs);

    start_mks = get_mks();
    memcpy_test(dst, src, samples);
    end_mks = get_mks();

    mks = end_mks - start_mks;
    secs = mks / 1000000.0;
    printf("HandMade memcpy: time %8lu mks size %4.2f MBytes speed %6.2f MB/s\n", mks, MBytes, MBytes / secs);

    start_mks = get_mks();
    TEST_TYPE s = sumsum(dst, samples);
    end_mks = get_mks();

    mks = end_mks - start_mks;
    secs = mks / 1000000.0;
    printf("HandMade sumsum: time %8lu mks size %4.2f MBytes speed %6.2f MB/s (sum %d)\n", mks, MBytes, MBytes / secs, s);

    start_mks = get_mks();
    memdmx(src, dst, dst1, samples);
    end_mks = get_mks();

    mks = end_mks - start_mks;
    secs = mks / 1000000.0;
    printf("HandMade  demux: time %8lu mks size %4.2f MBytes speed %6.2f MB/s\n", mks, MBytes, MBytes / secs);

    free(src);
    free(dst);
    free(dst1);
    return 0;
}
