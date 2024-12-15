#include "lib_func.h"

int main(int argc, char* argv[]) {
    int N, M;

    if (scanf("%d", &N) != 1 || N <= 0) {
        fprintf(stderr, "Некорректный размер матрицы.\n");
        return EXIT_FAILURE;
    }

    if (scanf("%d", &M) != 1 || M <= 0) {
        fprintf(stderr, "Некорректное число членов ряда.\n");
        return EXIT_FAILURE;
    }


    double* A = NULL;

    if (argc > 1 && !strcmp(argv[1], "random")) {
        A = matrix_generateRandomMatrix(N);
        if (N <= MAX_SIZE_OF_PRINTABLE_MATRIX){
            matrix_print(A, N);
        }
    } else {
        A = matrix_createZeroMatrix(N);
        printf("Введите элементы матрицы A: \n");
        for (int i = 0; i < N * N; ++i) {
            if (scanf("%lf", &A[i]) != 1) {
                fprintf(stderr, "Ошибка ввода элемента матрицы.\n");
                free(A);
                return EXIT_FAILURE;
            }
        }
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    double normA1 = matrix_norm1(A, N);
    double normAInf = matrix_normInf(A, N);
    double s = normA1 * normAInf;

    double* AT = matrix_transpose(A, N);
    double* B = matrix_scalarMultiply(1.0 / s, AT, N);
    free(AT); 

    double* BA = matrix_multiply(B, A, N);
    double* identity = matrix_createIdentityMatrix(N);
    double* BA_neg = matrix_scalarMultiply(-1.0, BA, N);
    free(BA);
    double* R = matrix_add(identity, BA_neg, N);
    free(BA_neg);
    free(identity);

    double normR = matrix_normInf(R, N);

    double* S = matrix_createIdentityMatrix(N);
    double* R_power = matrix_createZeroMatrix(N);
    memcpy(R_power, R, sizeof(double) * N * N); 

    for (int k = 1; k < M; ++k) {

        double* temp = matrix_add(S, R_power, N);
        free(S);
        S = temp;

        double* new_R_power = matrix_multiply(R_power, R, N);
        free(R_power);
        R_power = new_R_power;
    }
    free(R);

    double* A_inv = matrix_multiply(S, B, N);
    free(S);
    free(B);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_sec = end.tv_sec - start.tv_sec;
    double elapsed_nsec = end.tv_nsec - start.tv_nsec;
    double elapsed_time = elapsed_sec * 1000.0 + elapsed_nsec / 1.0e6;

    if (N <= MAX_SIZE_OF_PRINTABLE_MATRIX){
        matrix_print(A_inv, N);
    }

    printf("Время вычислений: %.3f ms\n", elapsed_time);

    free(A);
    free(A_inv);

    return 0;
}
