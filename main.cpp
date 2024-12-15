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

void fill_forward(int* array, size_t N) {
    for (size_t i = 0; i < N - 1; i++) {
        array[i] = static_cast<int>(i + 1);
    }
    array[N - 1] = 0;
}

void fill_reverse(int* array, size_t N) {
    for (size_t i = 1; i < N; i++) {
        array[i] = static_cast<int>(i - 1);
    }
    array[0] = static_cast<int>(N - 1);
}

void fill_random(int* array, size_t N) {
    std::vector<size_t> indices(N);
    for (size_t i = 0; i < N; i++) {
        indices[i] = i;
    }
    shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));
    for (size_t i = 0; i < N - 1; i++) {
        array[indices[i]] = static_cast<int>(indices[i + 1]);
    }
    array[indices[N - 1]] = static_cast<int>(indices[0]);
}

double measure_time(int* array, size_t N, size_t K) {
    volatile int k = 0;
    uint64_t start = rdtsc();
    for (size_t i = 0; i < N * K; i++) {
        k = array[k];
    }
    uint64_t end = rdtsc();
    return static_cast<double>(end - start) / (N * K);
}

int main() {
    size_t Nmin = 256;  
    size_t Nmax = 4 * 1024 * 1024;
    size_t K = 1000;
    size_t L2 = 7 * 1024 * 1024;
    int num_trials = 1;

    std::cout << "Размер массива\tПрямой обход\tОбратный обход\tСлучайный обход\n";
    std::vector<int> array(L2);
    fill_forward(array.data(), L2);
    fill_reverse(array.data(), L2);
    fill_random(array.data(), L2);

    for (size_t N = Nmin; N <= Nmax; N *= 2) {
        array.resize(N);
        fill_forward(array.data(), Nmin);
        double min_times[3] = {100, 100, 100};
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
        std::cout << (N * sizeof(int)) / 1024 << "\t\t"
             << min_times[0] << "\t\t"
             << min_times[1] << "\t\t"
             << min_times[2] << "\n";
    }

    return 0;
}
