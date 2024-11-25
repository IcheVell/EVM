#include "lib_func.h"

double* matrix_createZeroMatrix(int N) {
    double* matrix = (double*)calloc(N * N, sizeof(double));
    if (!matrix) {
        fprintf(stderr, "Ошибка выделения памяти для матрицы.\n");
        exit(EXIT_FAILURE);
    }
    return matrix;
}

double* matrix_createIdentityMatrix(int N) {
    double* identity = matrix_createZeroMatrix(N);
    for (int i = 0; i < N; ++i) {
        identity[i * N + i] = 1.0;
    }
    return identity;
}

double* matrix_transpose(const double* A, int N) {
    double* AT = matrix_createZeroMatrix(N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            AT[j * N + i] = A[i * N + j];
        }
    }
    return AT;
}

double* matrix_multiply(const double* A, const double* B, int N) {
    double* C = matrix_createZeroMatrix(N);
    const double alpha = 1.0;
    const double beta = 0.0;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                N, N, N, alpha,
                A, N,
                B, N,
                beta,
                C, N);
    return C;
}

double* matrix_add(const double* A, const double* B, int N) {
    double* C = matrix_createZeroMatrix(N);
    cblas_dcopy(N * N, A, 1, C, 1);
    cblas_daxpy(N * N, 1.0, B, 1, C, 1);
    return C;
}

double* matrix_scalarMultiply(double scalar, const double* A, int N) {
    double* B = matrix_createZeroMatrix(N);
    cblas_dcopy(N * N, A, 1, B, 1);
    cblas_dscal(N * N, scalar, B, 1);
    return B;
}

double* matrix_generateRandomMatrix(int N) {
    double* A = matrix_createZeroMatrix(N);
    srand(time(NULL));
    for (int i = 0; i < N * N; ++i) {
        A[i] = ((double)rand() / RAND_MAX) * 100.0;
    }
    return A;
}

double matrix_norm1(const double* A, int N) {
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

double matrix_normInf(const double* A, int N) {
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

void matrix_print(const double* A, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%10.6lf ", A[i * N + j]);
        }
        printf("\n");
    }
}
