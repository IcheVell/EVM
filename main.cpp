#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <random>
#include <x86intrin.h>

uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__("cpuid\n\t"
                         "rdtsc\n\t"
                         : "=a" (lo), "=d" (hi)
                         :: "%rbx", "%rcx");
    return (static_cast<uint64_t>(hi) << 32) | lo;
}

void fill_forward(size_t* array, size_t N) {
    for (size_t i = 0; i < N - 1; i++) {
        array[i] = i + 1;
    }
    array[N - 1] = 0;
}

void fill_reverse(size_t* array, size_t N) {
    for (size_t i = 1; i < N; i++) {
        array[i] = i - 1;
    }
    array[0] = N - 1;
}

void fill_random(size_t* array, size_t N) {
    std::vector<size_t> indices(N);
    for (size_t i = 0; i < N; i++) {
        indices[i] = i;
    }
    shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));
    for (size_t i = 0; i < N - 1; i++) {
        array[indices[i]] = indices[i + 1];
    }
    array[indices[N - 1]] = indices[0];
}

double measure_time(size_t* array, size_t N, size_t K) {
    volatile size_t k = 0;
    uint64_t start = rdtsc();
    for (size_t i = 0; i < N * K; i++) {
        k = array[k];
    }
    uint64_t end = rdtsc();
    return static_cast<double>(end - start) / (N * K);
}

int main() {
    size_t Nmin = 128;
    size_t Nmax = 32 * 1024 * 1024 / sizeof(size_t);
    size_t K = 100;
    size_t L2 = 7 * 1024 * 1024 / sizeof(size_t);
    int num_trials = 2;

    std::cout << "Размер массива\tПрямой обход\tОбратный обход\tСлучайный обход\n";
    std::vector<size_t> array(L2);
    fill_forward(array.data(), L2);
    fill_reverse(array.data(), L2);
    fill_random(array.data(), L2);

    for (size_t N = Nmin; N <= Nmax; N *= 1.4) {
        array.resize(N);
        double min_times[3] = {100000, 100000, 100000};
        for (int method = 0; method < 3; method++) {
            for (int trial = 0; trial < num_trials; trial++) {
                if (method == 0) {
                    fill_forward(array.data(), N);
                } else if (method == 1) {
                    fill_reverse(array.data(), N);
                } else {
                    fill_random(array.data(), N);
                }
                double time = measure_time(array.data(), N, K);

                if (time < min_times[method]) {
                    min_times[method] = time;
                }
            }
        }
        std::cout << (N * sizeof(size_t)) / 1024 << "\t\t"
                  << min_times[0] << "\t\t"
                  << min_times[1] << "\t\t"
                  << min_times[2] << "\n";
    }

    return 0;
}
