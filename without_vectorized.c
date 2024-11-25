#include "without_vectorized.h"

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
        A = generateRandomMatrix(N, time(NULL));
        if (N <= MAX_SIZE_OF_PRINTABLE_MATRIX){
            printMatrix(A, N, "Сгенерированная матрица A");
        }
    } else {
        A = createZeroMatrix(N);
        printf("Введите матрицу A: \n");
        for (int i = 0; i < N * N; ++i) {
            if (scanf("%lf", &A[i]) != 1) {
                free(A);
                return EXIT_FAILURE;
            }
        }
    }

    struct timespec start, end;
    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        free(A);
        return EXIT_FAILURE;
    }

    double normA1 = norm1(A, N);
    double normAInf = normInf(A, N);
    double s = normA1 * normAInf;

    double* AT = transpose(A, N);
    double* B = scalarMultiply(1.0 / s, AT, N);
    free(AT);

    double* BA = multiply(B, A, N);
    double* I = createIdentityMatrix(N);
    double* BA_neg = scalarMultiply(-1.0, BA, N);
    free(BA);

    double* R = add(I, BA_neg, N);
    free(BA_neg);
    free(I);

    double* S = createIdentityMatrix(N);
    double* R_power = createIdentityMatrix(N);


    for (int k = 1; k < M; ++k) {
        double* temp = multiply(R_power, R, N);
        double* new_S = add(S, temp, N);
        free(S);
        free(temp);
        S = new_S;
    }
    free(R_power);

    double* A_inv = multiply(S, B, N);
    free(S);
    free(B);

    if (clock_gettime(CLOCK_MONOTONIC, &end) == -1) {
        free(A_inv);
        free(A);
        return EXIT_FAILURE;
    }

    double elapsed_time = (end.tv_sec - start.tv_sec) * 1000.0;
    elapsed_time += (end.tv_nsec - start.tv_nsec) / 1.0e6;

    if (N <= 20){
        printMatrix(A_inv, N, "A_inv");
    }

    printf("Время вычислений: %.3lf ms\n", elapsed_time);

    free(A);
    free(R);
    free(A_inv);

    return EXIT_SUCCESS;
}
