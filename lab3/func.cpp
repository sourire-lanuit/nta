#include "Header.h"

long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    long long x1, y1;
    long long d = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}


long long mod_inv(long long a, long long m) {
    long long x, y;
    long long g = ext_gcd(a, m, x, y);
    if (g != 1) return -1; 
    return (x % m + m) % m;
}

long long mod_exp(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

std::vector<int> factor_base(int B) {
    std::vector<int> primes;
    std::vector<bool> is_prime(B + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int p = 2; p * p <= B; p++) {
        if (is_prime[p]) {
            for (int i = p * p; i <= B; i += p)
                is_prime[i] = false;
        }
    }
    for (int p = 2; p <= B; p++) {
        if (is_prime[p]) primes.push_back(p);
    }
    return primes;
}