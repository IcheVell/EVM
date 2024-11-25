#include "man_vectorized_func.h"

double* createZeroMatrix(int N) {
    double* matrix;
    if (posix_memalign((void**)&matrix, 16, N * N * sizeof(double)) != 0) {
        fprintf(stderr, "Ошибка выделения памяти для матрицы.\n");
        exit(EXIT_FAILURE);
    }
    memset(matrix, 0, N * N * sizeof(double));
    return matrix;
}

double* createIdentityMatrix(int N) {
    double* I = createZeroMatrix(N);
    for (int i = 0; i < N; ++i) {
        I[i * N + i] = 1.0;
    }
    return I;
}

void transpose(double* A, double* AT, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            AT[j * N + i] = A[i * N + j];
        }
    }
}

void multiply(double* A, double* B, double* C, int N) {
    memset(C, 0, N * N * sizeof(double));
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            double a = A[i * N + k];
            v2df a_vec = {a, a};
            int j = 0;
            for (; j <= N - 2; j += 2) {
                v2df b_vec = {B[k * N + j], B[k * N + j + 1]};
                v2df c_vec = {C[i * N + j], C[i * N + j + 1]};
                c_vec += a_vec * b_vec;
                C[i * N + j] = c_vec[0];
                C[i * N + j + 1] = c_vec[1];
            }
            for (; j < N; ++j) {
                C[i * N + j] += a * B[k * N + j];
            }
        }
    }
}

void add(double* A, double* B, double* C, int N) {
    int size = N * N;
    int i = 0;
    for (; i <= size - 2; i += 2) {
        v2df a_vec = {A[i], A[i + 1]};
        v2df b_vec = {B[i], B[i + 1]};
        v2df c_vec = a_vec + b_vec;
        C[i] = c_vec[0];
        C[i + 1] = c_vec[1];
    }
    for (; i < size; ++i) {
        C[i] = A[i] + B[i];
    }
}

void scalarMultiply(double scalar, double* A, double* B, int N) {
    int size = N * N;
    int i = 0;
    v2df scalar_vec = {scalar, scalar};
    for (; i <= size - 2; i += 2) {
        v2df a_vec = {A[i], A[i + 1]};
        v2df b_vec = scalar_vec * a_vec;
        B[i] = b_vec[0];
        B[i + 1] = b_vec[1];
    }
    for (; i < size; ++i) {
        B[i] = scalar * A[i];
    }
}

double* generateRandomMatrix(int N) {
    double* A = createZeroMatrix(N);
    for (int i = 0; i < N * N; ++i) {
        A[i] = ((double)rand() / RAND_MAX) * 100.0;
    }
    return A;
}

double norm1(double* A, int N) {
    double maxSum = 0.0;
    for (int j = 0; j < N; ++j) {
        double columnSum = 0.0;
        for (int i = 0; i < N; ++i) {
            columnSum += fabs(A[i * N + j]);
        }
        if (columnSum > maxSum) {
            maxSum = columnSum;
        }
    }
    return maxSum;
}

double normInf(double* A, int N) {
    double maxSum = 0.0;
    for (int i = 0; i < N; ++i) {
        double rowSum = 0.0;
        for (int j = 0; j < N; ++j) {
            rowSum += fabs(A[i * N + j]);
        }
        if (rowSum > maxSum) {
            maxSum = rowSum;
        }
    }
    return maxSum;
}

void printMatrix(double* A, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%10.6f ", A[i * N + j]);
        }
        printf("\n");
    }
}