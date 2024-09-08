#include "header.h"

double findPi(int n) {
    double pi = 0;
    int index = 0;
    while (index <= n) {
        if (index % 2 == 0) {
            pi += 4 *(pow(-1, index) / (2 * index + 1));
        } else {
            pi += 4 *(pow(-1, index) / (2 * index + 1));
        }
        index++;
    }
    return pi;
}

