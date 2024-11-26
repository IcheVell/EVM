#include "without_vectorized.h"

double* createZeroMatrix(int N) {
    double* matrix = (double*)calloc(N * N, sizeof(double));
    if (matrix == NULL) {
        exit(EXIT_FAILURE);
    }
    return matrix;
}

double* createIdentityMatrix(int N) {
    double* I = createZeroMatrix(N);
    for (int i = 0; i < N; ++i) {
        I[i * N + i] = 1.0;
    }
    return I;
}

double* transpose(const double* A, int N) {
    double* AT = createZeroMatrix(N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            AT[j * N + i] = A[i * N + j];
        }
    }
    return AT;
}

double* multiply(const double* A, const double* B, int N) {
    double* C = createZeroMatrix(N);
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            double a_ik = A[i * N + k];
            for (int j = 0; j < N; ++j) {
                C[i * N + j] += a_ik * B[k * N + j];
            }
        }
    }
    return C;
}

double* add(const double* A, const double* B, int N) {
    double* C = createZeroMatrix(N);
    for (int i = 0; i < N * N; ++i) {
        C[i] = A[i] + B[i];
    }
    return C;
}

double* scalarMultiply(double scalar, const double* A, int N) {
    double* B = createZeroMatrix(N);
    for (int i = 0; i < N * N; ++i) {
        B[i] = scalar * A[i];
    }
    return B;
}

double norm1(const double* A, int N) {
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

double normInf(const double* A, int N) {
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

void printMatrix(const double* A, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%10.6lf ", A[i * N + j]);
        }
        printf("\n");
    }
}

double* generateRandomMatrix(int N, unsigned int seed) {
    double* A = createZeroMatrix(N);
    srand(seed);
    for (int i = 0; i < N * N; ++i) {
        A[i] = ((double)rand() / RAND_MAX) * 100.0;
    }
    return A;
}