#ifndef MAN_VECTORIZED_FUNC_H
#define MAN_VECTORIZED_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE_OF_PRINTABLE_MATRIX 15

typedef double v2df __attribute__ ((vector_size (16)));

double* createZeroMatrix(int N);
double* createIdentityMatrix(int N);
void transpose(double* A, double* AT, int N);
void multiply(double* A, double* B, double* C, int N);
void add(double* A, double* B, double* C, int N);
void scalarMultiply(double scalar, double* A, double* B, int N);
double* generateRandomMatrix(int N);
double norm1(double* A, int N);
double normInf(double* A, int N);
void printMatrix(double* A, int N);

#endif