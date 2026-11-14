#include "Header.h"

uint64_t pollard(uint64_t n) {
    if (n < 4) return 1;
    if (n % 2 == 0) return 2;

    static std::mt19937_64 rng(std::random_device{}());

    uint64_t x = 2, y = 2, d = 1, c = 1;
    
    auto f = [&](uint64_t val) {
         return (uint64_t)(((unsigned __int128)val * val + c) % n);
    };

    while (d == 1) {
        x = f(x);
        y = f(f(y));
        uint64_t diff = (x > y) ? (x - y) : (y - x);
        d = gcd(diff, n);
        if (d == n) {
            x = rng() % (n - 2) + 2;
            y = x;
            c = rng() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}