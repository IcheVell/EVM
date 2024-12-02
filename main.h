#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#if defined(__GNUC__) || defined(__GNUG__)
#include <x86intrin.h>
#endif

#define N_MIN (256)
#define N_MAX (32 * 1024 * 1024)
#define K (100)

void fill_forward(int *x, int N);
void fill_reverse(int *x, int N);
void fill_random(int *x, int N);
double measure_time(int *x, int N);

#endif