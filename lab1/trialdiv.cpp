#include "Header.h"

uint64_t trial_division(uint64_t n) {
    uint64_t primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    if (n < 2) return 1;
    for (uint64_t p : primes) {
        if (n % p == 0) {
            return p;
        }
    }
    return 1; 
}