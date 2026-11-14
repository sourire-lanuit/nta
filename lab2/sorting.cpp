#include "Header.h"

long long power_mod(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

long long perebir(long long alpha, long long beta, long long p) {
    for (long long x = 0; x <= p - 2; ++x) {
        if (power_mod(alpha, x, p) == beta) {
            return x;
        }
    }
    return -1; 
}