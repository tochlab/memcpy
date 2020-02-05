#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <malloc.h>

#define TEST_TYPE int

inline unsigned long get_mks() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void memcpy_test(const TEST_TYPE* src, TEST_TYPE* dst, int len) {
    for (int i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}

int main() {
    int len = 128 * 1024 * 1024;
    printf("Allocating %lu bytes\n", len * sizeof(TEST_TYPE));
    TEST_TYPE* src = malloc(len * sizeof(TEST_TYPE));
    TEST_TYPE* dst = malloc(len * sizeof(TEST_TYPE));

    srand(len);
    for (int i = 0; i < len; i++) {
        src[i] = rand();
    }

    unsigned long start_mks = get_mks();
    memcpy_test(src, dst, len);
    unsigned long end_mks = get_mks();

    unsigned long mks = end_mks - start_mks;
    double MBytes = (((double) len * sizeof(TEST_TYPE)) / (1024 * 1024.0f));
    double secs = mks / 1000000.0f;

    printf("Time %lu mks size %.2f MBytes speed %.2f MB/s\n", mks, MBytes, MBytes / secs);

    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += dst[i];
    }

    printf("Sum %d\n", sum);

    free(src);
    free(dst);
    return 0;
}
