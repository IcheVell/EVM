#ifndef WITHOUT_VECTORIZED_H
#define WITHOUT_VECTORIZED_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_SIZE_OF_PRINTABLE_MATRIX 15


double* createZeroMatrix(int N);
double* createIdentityMatrix(int N);
double* transpose(const double* A, int N);
double* multiply(const double* A, const double* B, int N);
double* add(const double* A, const double* B, int N);
double* scalarMultiply(double scalar, const double* A, int N);
double norm1(const double* A, int N);
double normInf(const double* A, int N);
void printMatrix(const double* A, int N, const char* description);
double* generateRandomMatrix(int N, unsigned int seed);

#endif