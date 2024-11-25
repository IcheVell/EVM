#ifndef LIB_FUNC_H
#define LIB_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cblas.h>
#include <time.h>
#include <stdbool.h>

#define MAX_SIZE_OF_PRINTABLE_MATRIX 15

double* matrix_createZeroMatrix(int N);
double* matrix_createIdentityMatrix(int N);
double* matrix_transpose(const double* A, int N);
double* matrix_multiply(const double* A, const double* B, int N);
double* matrix_add(const double* A, const double* B, int N);
double* matrix_scalarMultiply(double scalar, const double* A, int N);
double* matrix_generateRandomMatrix(int N);
double matrix_norm1(const double* A, int N);
double matrix_normInf(const double* A, int N);
void matrix_print(const double* A, int N);

#endif