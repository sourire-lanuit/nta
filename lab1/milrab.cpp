#include "Header.h"
#include <cstdlib>

bool miller_rabin(uint64_t n, int k) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    uint64_t d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    for (int i = 0; i < k; i++) {
        uint64_t a = 2 + rand() % (n - 3);
        uint64_t x = power_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool compos = true;
        for (int r = 1; r < s; r++) {
            x = mul_mod(x, x, n);
            if (x == 1) return false;
            if (x == n - 1) {
                compos = false;
                break;
            }
        }
        if (compos) return false;
    }
    return true;
}