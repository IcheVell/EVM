#include <stdio.h>
#include <time.h>

int main() {
    struct timespec res;
    if (clock_getres(CLOCK_MONOTONIC, &res) == 0) {
        printf("%ld.%09ld", res.tv_sec, res.tv_nsec);
    }
    return 0;
}