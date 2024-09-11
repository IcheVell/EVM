#include "header.h"

int main() {
    long n = 5;
    std::cout << "Pi equals " << findPi(n) << " with n = " << n << std::endl;
    n = 100;
    std::cout << "Pi equals " << findPi(n) << " with n = " << n << std::endl;
    n = 2134;
    std::cout << "Pi equals " << findPi(n) << " with n = " << n << std::endl;
    n = 2100000000;
    std::cout << "Pi equals " << findPi(n) << " with n = " << n << std::endl;
    n = 2100000000;
    std::cout << "Pi equals " << findPi(n) << " with n = " << n;
    return 0;
}