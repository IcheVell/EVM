#include "header.h"

double findPi(int n) {
    double pi = 0;
    int index = 0;
    while (index <= n) {
        pi += 4 *(pow(-1, index) / (2 * index + 1));
        index++;
    }
    return pi;
}

