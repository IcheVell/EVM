#include "main.h"

void fill_forward(int *x, int N) {
    for (int i = 0; i < N - 1; i++)
        x[i] = i + 1;
    x[N - 1] = 0;
}

void fill_reverse(int *x, int N) {
    for (int i = N - 1; i > 0; i--)
        x[i] = i - 1;
    x[0] = N - 1;
}

void fill_random(int *x, int N) {

    for (int i = 0; i < N; i++)
        x[i] = i;


    for (int i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = x[i];
        x[i] = x[j];
        x[j] = temp;
    }


    int *visited = (int *)calloc(N, sizeof(int));
    int current = x[0];
    visited[current] = 1;
    for (int i = 1; i < N; i++) {
        int next = x[i];
        x[current] = next;
        visited[next] = 1;
        current = next;
    }
    x[current] = x[0];

    free(visited);
}

double measure_time(int *x, int N) {
    volatile int k = 0;
    uint64_t start, end;

    for (int i = 0; i < N; i++)
        k = x[k];

#if defined(__GNUC__) || defined(__GNUG__)
    start = __rdtsc();
    for (int i = 0; i < N * K; i++)
        k = x[k];
    end = __rdtsc();
#else
    struct timespec ts_start, ts_end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts_start);
    for (int i = 0; i < N * K; i++)
        k = x[k];
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts_end);
    start = ts_start.tv_sec * 1e9 + ts_start.tv_nsec;
    end = ts_end.tv_sec * 1e9 + ts_end.tv_nsec;
#endif

    if (k == 10)
        printf("k = %d\n", k);

    double avg_time_per_element = (double)(end - start) / (N * K);
    return avg_time_per_element;
}

int main() {
    srand(time(NULL));

    printf("Size(Bytes),Forward(ns),Reverse(ns),Random(ns)\n");
    for (int N = N_MIN; N <= N_MAX; N *= 2) {
        int *x = (int *)malloc(N * sizeof(int));
        if (x == NULL) {
            fprintf(stderr, "Memory allocation failed for N = %d\n", N);
            exit(EXIT_FAILURE);
        }

        fill_forward(x, N);
        double time_forward = measure_time(x, N);


        fill_reverse(x, N);
        double time_reverse = measure_time(x, N);

        fill_random(x, N);
        double time_random = measure_time(x, N);

        printf("%d,%.2f,%.2f,%.2f\n", N * (int)sizeof(int), time_forward, time_reverse, time_random);

        free(x);
    }
    return 0;
}
