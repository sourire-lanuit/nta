#include "Header.h"
#include <cstdlib>

uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t x = 2, y = 2, d = 1, c = 1;
    
    auto f = [&](uint64_t val) {
        return (mul_mod(val, val, n) + c) % n;
    };

    while (d == 1) {
        x = f(x);
        y = f(f(y));
        uint64_t diff = (x > y) ? (x - y) : (y - x);
        d = gcd(diff, n);
        if (d == n) {

            x = rand() % (n - 2) + 2;
            y = x;
            c = rand() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}