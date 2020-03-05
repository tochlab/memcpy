#include <cstring>
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>

#define TEST_TYPE int

inline unsigned long get_mks() {
    auto mks_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
    );
    return (uint64_t) mks_since_epoch.count();
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

void show_stat(const std::string &test_name, unsigned long mks, double mbytes, TEST_TYPE sum = 0) {
    std::cout << std::setprecision(1) << std::fixed;
    std::cout << std::setw(15);
    std::cout << test_name << ": time ";
    std::cout << std::setw(8) << mks <<" mks ";
    std::cout << "size " << mbytes << " MBytes ";
    std::cout << "speed " << std::setw(8) << mbytes / mks * 1000000 << "MB/s";

    if (sum != 0) {
        std::cout << " (sum " << sum << ")";
    }

    std::cout << std::endl;
}

int main() {
    size_t samples = 128 * 1024 * 1024;
    std::cout << "Allocating " << samples * sizeof(TEST_TYPE) << " bytes";
    size_t alloc_len8 = samples * sizeof(TEST_TYPE);

    auto src = new TEST_TYPE[alloc_len8];
    auto dst0 = new TEST_TYPE[alloc_len8];
    auto dst1 = new TEST_TYPE[alloc_len8];

    std::cout << " src " << src << " dst0 " << dst0 << " dst1 " << dst1 << std::endl;

    double MBytes = (((double) alloc_len8) / (1024 * 1024.0));

    std::default_random_engine generator(samples);
    std::uniform_int_distribution<int> distribution(1,samples);

    for (size_t i = 0; i < samples; i++) {
        src[i] = distribution(generator);
        dst0[i] = distribution(generator);
        dst1[i] = distribution(generator);
    }

    std::cout << "Sleeping..." << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));

    std::cout << "Running!" << std::endl;
    unsigned long start_mks = get_mks();
    ::memcpy(dst0, src, alloc_len8);
    unsigned long end_mks = get_mks();

    show_stat("Native memcpy", end_mks - start_mks, MBytes);

    start_mks = get_mks();
    memcpy_test(dst0, src, samples);
    end_mks = get_mks();

    show_stat("HandMade memcpy", end_mks - start_mks, MBytes);

    start_mks = get_mks();
    TEST_TYPE s = sumsum(dst0, samples);
    end_mks = get_mks();

    show_stat("HandMade sumsum", end_mks - start_mks, MBytes, s);

    start_mks = get_mks();
    memdmx(src, dst0, dst1, samples);
    end_mks = get_mks();

    show_stat("HandMade memdmx", end_mks - start_mks, MBytes);

    delete [] src;
    delete [] dst0;
    delete [] dst1;

    return 0;
}
