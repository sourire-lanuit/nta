#include "Header.h"

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((__int128)a * b % m);
}

uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = mul_mod(res, base, mod);
        base = mul_mod(base, base, mod);
        exp /= 2;
    }
    return res;
}

uint64_t gcd(uint64_t a, uint64_t b) {
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}