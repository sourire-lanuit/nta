#include "Header.h"
#include <cmath>
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>

uint64_t is_sqrt(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x = n;
    uint64_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

std::vector<int> factor_base(uint64_t n, int bound, int k = 10) {
    std::vector<int> base;
    base.push_back(-1);
    for (int p = 2; p <= bound; ++p) {
        if (miller_rabin(p, k)) {
            if (power_mod(n, (p - 1) / 2, p) == 1) {
                base.push_back(p);
            }
        }
    }
    return base;
}
std::vector<uint64_t> fraction_sqrt(uint64_t n, int lim = 200) {
    uint64_t u = 0, v = 1, a0 = is_sqrt(n);
    uint64_t a = a0;
    std::vector<uint64_t> terms;
    terms.push_back(a0);
    
    for (int i = 0; i < lim; ++i) {
        u = v * a - u;
        v = (n - u * u) / v;
        if (v == 0) break;
        a = (a0 + u) / v;
        terms.push_back(a);
    }
    return terms;
}

bool b_smooth(long long val, const std::vector<int>& base, std::map<int, int>& factors) {
    factors.clear();
    long long temp = val;
    
    if (temp < 0) {
        if (find(base.begin(), base.end(), -1) != base.end()) {
            factors[-1] = 1;
        }
        temp = -temp;
    }

    for (int p : base) {
        if (p == -1) continue;
        while (temp % p == 0) {
            temp /= p;
            factors[p]++;
        }
    }
    return temp == 1;
}

uint64_t brillhart_morrison(uint64_t n) {
    auto start_time = std::chrono::system_clock::now();
    double max_a = 6.0;
    double step = 0.25;
    double time_limit = 60.0;
    double a_val = 1.0 / sqrt(2.0); 

    while (a_val <= max_a) {
        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - start_time;
        if (elapsed.count() > time_limit) {
            std::cout << "МБМ не знайшов дільник за відведений час (" << time_limit << " с).\n";
            return 1;
        }

        double ln_n = log(n);
        double ln_ln_n = log(ln_n);
        int L = (int)exp(a_val * sqrt(ln_n * ln_ln_n));
        
        std::vector<int> base = factor_base(n, L, 10);
        std::vector<uint64_t> a_seq = fraction_sqrt(n, 300);
        
        if (a_seq.size() < 2) return 1;

        uint64_t b1 = 1;
        uint64_t b2 = a_seq[1];
        
        std::vector<uint64_t> bi_list;
        std::vector<std::vector<int>> exp_vec;
        std::map<std::vector<int>, int> last_vec;

        for (size_t i = 2; i < a_seq.size(); ++i) {
            elapsed = std::chrono::system_clock::now() - start_time;
            if (elapsed.count() > time_limit) return 1;

            uint64_t bi = ((__int128)a_seq[i] * b2 + b1) % n;
            b1 = b2;
            b2 = bi;
            
            uint64_t value = mul_mod(bi, bi, n);
            
            std::map<int, int> factors;
            if (!b_smooth((long long)value, base, factors)) {
                continue;
            }

            std::vector<int> whole_vec(base.size(), 0);
            std::vector<int> mod2_vec(base.size(), 0);
            bool all_zero = true;

            for (size_t j = 0; j < base.size(); ++j) {
                whole_vec[j] = factors[base[j]];
                mod2_vec[j] = whole_vec[j] % 2;
                if (mod2_vec[j] != 0) all_zero = false;
            }

            if (all_zero) {
                uint64_t X = bi % n;
                uint64_t Y = is_sqrt(value) % n;
                
                uint64_t delta1 = (X >= Y) ? (X - Y) : (X + (n - Y));
                uint64_t delta2 = (X + Y) % n;
                
                uint64_t d1 = gcd(delta1, n);
                if (d1 > 1 && d1 < n) return d1;
                
                uint64_t d2 = gcd(delta2, n);
                if (d2 > 1 && d2 < n) return d2;
                
                continue;
            }

            if (last_vec.find(mod2_vec) != last_vec.end()) {
                int j = last_vec[mod2_vec];
                uint64_t X = mul_mod(bi, bi_list[j], n);
                
                uint64_t Y_squared = 1;
                for (size_t k = 0; k < base.size(); ++k) {
                    int exp_sum = whole_vec[k] + exp_vec[j][k];
                    uint64_t part = power_mod((uint64_t)base[k], exp_sum / 2, n);
                    Y_squared = mul_mod(Y_squared, part, n);
                }
                
                uint64_t Y = Y_squared % n;
                
                uint64_t delta1 = (X >= Y) ? (X - Y) : (X + (n - Y));
                uint64_t delta2 = (X + Y) % n;
                
                uint64_t d1 = gcd(delta1, n);
                if (d1 > 1 && d1 < n) return d1;
                
                uint64_t d2 = gcd(delta2, n);
                if (d2 > 1 && d2 < n) return d2;
                
                continue;
            }

            last_vec[mod2_vec] = bi_list.size();
            bi_list.push_back(bi);
            exp_vec.push_back(whole_vec);
        }

        a_val += step;
    }

    return 1;
}