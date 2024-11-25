#include "man_vectorized_func.h"

int main(int argc, char* argv[]) {
    int N, M;

    printf("N: ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Неккоректный ввод N\n");
        return EXIT_FAILURE;
    }

    printf("M: ");
    if (scanf("%d", &M) != 1 || M <= 0 || M != N) {
        printf("Неккоректный ввод M\n");
        return EXIT_FAILURE;
    }

    double* A = NULL;

    if (argc > 1 && !strcmp(argv[1], "random")) {
        srand(time(NULL));
        A = generateRandomMatrix(N);
        if (N <=  MAX_SIZE_OF_PRINTABLE_MATRIX){
            printf("A: \n");
            printMatrix(A, N);
        }
    } else {
        A = createZeroMatrix(N);
        printf("Введите элементы матрицы A: \n");
        for (int i = 0; i < N * N; ++i) {
            if (scanf("%lf", &A[i]) != 1) {
                free(A);
                return EXIT_FAILURE;
            }
        }
    }

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    double normA1 = norm1(A, N);
    double normAInf = normInf(A, N);
    double s = normA1 * normAInf;

    double* AT = createZeroMatrix(N);
    transpose(A, AT, N);
    double* B = createZeroMatrix(N);
    scalarMultiply(1.0 / s, AT, B, N);

    double* BA = createZeroMatrix(N);
    multiply(B, A, BA, N);
    double* I = createIdentityMatrix(N);
    double* BA_neg = createZeroMatrix(N);
    scalarMultiply(-1.0, BA, BA_neg, N);
    double* R = createZeroMatrix(N);
    add(I, BA_neg, R, N);

    double normR = normInf(R, N);

    double* S = createIdentityMatrix(N);
    double* R_power = createZeroMatrix(N);
    memcpy(R_power, R, N * N * sizeof(double));
    double* temp = createZeroMatrix(N);

    for (int k = 1; k < M; ++k) {

        add(S, R_power, temp, N);
        memcpy(S, temp, N * N * sizeof(double));

        multiply(R_power, R, temp, N);
        memcpy(R_power, temp, N * N * sizeof(double));
    }

    double* A_inv = createZeroMatrix(N);
    multiply(S, B, A_inv, N);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_sec = end.tv_sec - start.tv_sec;
    double elapsed_nsec = end.tv_nsec - start.tv_nsec;
    double elapsed_time = elapsed_sec * 1000.0 + elapsed_nsec / 1.0e6;

    if (N <= 20){
        printf("Обратная матрица: \n");
        printMatrix(A_inv, N);
    } 

    printf("Время вычислений: %.3f ms\n", elapsed_time);

    free(A);
    free(AT);
    free(B);
    free(BA);
    free(I);
    free(BA_neg);
    free(R);
    free(S);
    free(R_power);
    free(temp);
    free(A_inv);

    return 0;
}
