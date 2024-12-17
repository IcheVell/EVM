#include <iostream>
#include <vector>
#include <x86intrin.h>
#include <cstdlib>
#include <cstdint>

uint64_t ReadTimeStampCounter() {
    unsigned int low, high;
    __asm__ __volatile__ (
        "cpuid\n\t"     
        "rdtsc\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t"
        : "=r" (high), "=r" (low)
        :: "%rax", "%rbx", "%rcx", "%rdx"
    );
    return ((uint64_t)high << 32) | low;
}

void RunCacheLatencyTest(
    size_t maxFragments = 32,
    size_t trialCount = 1000000
) {
    size_t offsetBytes = 16 * 1024 * 1024;
    size_t fragmentSize = offsetBytes / sizeof(size_t);

    std::cout << "Fragments\tAverage Access Time (cycles)" << std::endl;

    for (size_t fragmentCount = 1; fragmentCount <= maxFragments; fragmentCount++) {
        size_t arraySize = fragmentCount * fragmentSize;

        std::vector<size_t> dataArray(arraySize, 0);

        for (size_t index = 0; index < arraySize; index++) {
            size_t currentFragment = index / fragmentSize;
            size_t nextFragment = (currentFragment + 1) % fragmentCount;
            size_t offsetWithinFragment = index % fragmentSize;

            size_t nextIndex = nextFragment * fragmentSize + offsetWithinFragment;
            if (nextIndex >= arraySize)
                nextIndex = 0;
            dataArray[index] = nextIndex;
        }

        volatile size_t currentIndex = 0;

        for (size_t index = 0; index < arraySize; index++) {
            currentIndex = dataArray[currentIndex];
        }

        __asm__ __volatile__("mfence" ::: "memory");
        uint64_t startCycles = ReadTimeStampCounter();
        for (size_t trial = 0; trial < trialCount; trial++) {
            currentIndex = dataArray[currentIndex];
        }
        uint64_t endCycles = ReadTimeStampCounter();
        __asm__ __volatile__("mfence" ::: "memory");

        double averageCycles = static_cast<double>(endCycles - startCycles) / trialCount;
        std::cout << fragmentCount << "\t\t" << averageCycles << std::endl;
    }
}

int main() {
    RunCacheLatencyTest();
    return 0;
}
