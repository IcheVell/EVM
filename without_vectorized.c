#include "without_vectorized.h"

int main(int argc, char* argv[]) {
    int N, M;

    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Неккоректный ввод N\n");
        return EXIT_FAILURE;
    }

    if (scanf("%d", &M) != 1 || M <= 0) {
        printf("Неккоректный ввод M\n");
        return EXIT_FAILURE;
    }

    double* A = NULL;

    if (argc > 1 && !strcmp(argv[1], "random")) {
        A = generateRandomMatrix(N, time(NULL));
        if (N <= MAX_SIZE_OF_PRINTABLE_MATRIX){
            printMatrix(A, N);
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
    double* temp_power = multiply(R_power, R, N); 
    free(R_power);
    R_power = temp_power;

    double* new_S = add(S, R_power, N);
    free(S);
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

    if (N <= MAX_SIZE_OF_PRINTABLE_MATRIX){
        printf("A_inv: \n");
        printMatrix(A_inv, N);
    }

    printf("Время вычислений: %.3lf ms\n", elapsed_time);

    double* m_check = multiply(A, A_inv, N);

    if (isIdentityMatrix(m_check, N)) {
        printf("Обратная матрица корректна\n");
    } else {
        printf("Обратная матрица некорректна\n");
    }

    free(A);
    free(R);
    free(A_inv);

    return EXIT_SUCCESS;
}
