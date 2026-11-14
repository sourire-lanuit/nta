#include "Header.h"

long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }
    long long x1, y1;
    long long gcd = ext_gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

long long mod_inv(long long a, long long m) {
    long long x, y;
    long long g = ext_gcd(a, m, x, y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

long long silv_pol_hel(long long alpha, long long beta, long long p) {
    long long n = p - 1;
    std::map<long long, long long> factors;
    long long temp_n = n;
    
    for (long long i = 2; i * i <= temp_n; i++) {
        while (temp_n % i == 0) {
            factors[i]++;
            temp_n /= i;
        }
    }
    if (temp_n > 1) {
        factors[temp_n]++;
    }

    std::vector<long long> mods;
    std::vector<long long> res;

    for (auto const& [q, e] : factors) {
        long long q_e = pow(q, e);
        mods.push_back(q_e);

        std::map<long long, long long> table;
        long long alpha_n_q = power_mod(alpha, n / q, p);
        long long current_val = 1;
        for (long long j = 0; j < q; j++) {
            table[current_val] = j;
            current_val = (current_val * alpha_n_q) % p;
        }

        long long x = 0;
        long long current_beta = beta;
        long long q_pow = 1;

        for (long long k = 0; k < e; k++) {
            long long exp = n / (q_pow * q);
            long long beta_part = power_mod(current_beta, exp, p);
            
            if (table.find(beta_part) == table.end()) {
                return -1;
            }
            long long x_k = table[beta_part];
            x += x_k * q_pow;

            long long alpha_inv = mod_inv(alpha, p);
            long long alpha_xk_inv = power_mod(alpha_inv, x_k * q_pow, p);
            current_beta = (current_beta * alpha_xk_inv) % p;
            q_pow *= q;
        }
        res.push_back(x);
    }

    long long result = 0;
    for (size_t i = 0; i < mods.size(); i++) {
        long long m_i = n / mods[i];
        long long m_i_inv = mod_inv(m_i, mods[i]);
        long long term = (res[i] * m_i) % n;
        term = (term * m_i_inv) % n;
        result = (result + term) % n;
    }

    return (result % n + n) % n;
}