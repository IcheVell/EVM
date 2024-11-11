#include <stdio.h>
#include <stdint.h>

int main() {
    int32_t a[] = {0, -1074790400};
    double b = *((double *)a);
    printf("%lf\n", b);
    return 0;
}