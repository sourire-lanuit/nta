#include "Header.h"

std::vector<int> factor_base(uint64_t n, int bound) {
    std::vector<int> base;
    base.push_back(-1);
    std::vector<bool> compos(bound + 1, false);
    for (long long p = 2; p <= bound; ++p) {
        if (compos[p]) continue;
        for (long long j = p * p; j <= bound; j += p) compos[j] = true;
        if (p == 2 || power_mod(n % p, (p - 1) / 2, p) == 1) base.push_back((int)p);
    }
    return base;
}

bool b_smooth(uint64_t q, const std::vector<int>& base, std::vector<std::pair<int,int>>& expo) {
    expo.clear();
    for (size_t i = 1; i < base.size() && q > 1; ++i) {
        uint64_t p = (uint64_t)base[i];
        if (q % p) continue;
        int e = 0;
        while (q % p == 0) { q /= p; ++e; }
        expo.push_back({(int)i, e});
    }
    return q == 1;
}

typedef std::vector<uint64_t> Bits;

static void xor_into(Bits& a, const Bits& b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    for (size_t i = 0; i < b.size(); ++i) a[i] ^= b[i];
}
static void set_bit(Bits& a, size_t i) {
    if (a.size() <= i / 64) a.resize(i / 64 + 1, 0);
    a[i / 64] |= (1ULL << (i % 64));
}
static int lowest_bit(const Bits& a) {
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i]) return (int)(i * 64 + __builtin_ctzll(a[i]));
    return -1;
}

struct Relation {
    uint64_t A; 
    std::vector<std::pair<int,int>> expo;
};

static uint64_t find_solution(const Bits& h, const std::vector<Relation>& rels, const std::vector<int>& base, uint64_t n) {
    std::vector<long long> gamma(base.size(), 0);
    uint64_t X = 1;
    for (size_t w = 0; w < h.size(); ++w) {
        uint64_t word = h[w];
        while (word) {
            int b = __builtin_ctzll(word);
            word &= word - 1;
            const Relation& r = rels[w * 64 + b];
            X = mul_mod(X, r.A, n);
            for (auto& pe : r.expo) gamma[pe.first] += pe.second;
        }
    }
    uint64_t Y = 1;
    for (size_t j = 1; j < base.size(); ++j) {
        if (gamma[j] >= 2) Y = mul_mod(Y, power_mod((uint64_t)base[j], (uint64_t)(gamma[j] / 2), n), n);
    }
    if (X == Y || X == (n - Y) % n) return 1;
    uint64_t diff = (X >= Y) ? (X - Y) : (Y - X);
    uint64_t d = gcd(diff, n);
    if (d > 1 && d < n) return d;
    d = gcd((uint64_t)(((unsigned __int128)X + Y) % n), n);
    if (d > 1 && d < n) return d;
    return 1;
}

uint64_t brillhart_morrison(uint64_t n, double time_limit) {
    auto start_time = std::chrono::steady_clock::now();
    auto elapsed = [&]() {
        return std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time).count();
    };

    if (n < 4) return 1;
    if (n % 2 == 0) return 2;
    uint64_t a0 = is_sqrt(n);
    if ((unsigned __int128)a0 * a0 == n) return a0;

    const double max_a = 6.0, step = 0.25;
    const double ln_n = std::log((double)n);
    const double ln_ln_n = std::log(ln_n);

    for (double a_val = 1.0 / std::sqrt(2.0); a_val <= max_a; a_val += step) {
        double Ld = std::exp(a_val * std::sqrt(ln_n * ln_ln_n));
        int L = (int)std::min(Ld, 3.0e6);
        if (L < 10) L = 10;

        for (int p = 2; p <= L; ++p)
            if (n % p == 0 && (uint64_t)p < n) return (uint64_t)p;

        std::vector<int> base = factor_base(n, L);
        size_t m = base.size();
        size_t max_terms = 50 * m + 5000;

        std::vector<Relation> rels;
        std::vector<Bits> pivot_vec(m), pivot_hist(m);
        std::vector<bool> has_pivot(m, false);
        std::vector<Bits> solutions;

        uint64_t u = 0, v = 1, a = a0;
        uint64_t A_prev = 1, A_cur = a0 % n;

        for (size_t k = 1; k <= max_terms; ++k) {
            if ((k & 1023) == 0 && elapsed() > time_limit) return 1;

            u = a * v - u;
            v = (n - u * u) / v;
            if (v == 0) break;
            a = (a0 + u) / v;

            std::vector<std::pair<int,int>> expo;
            if (b_smooth(v, base, expo)) {
                Relation r;
                r.A = A_cur;
                if (k & 1) r.expo.push_back({0, 1});
                r.expo.insert(r.expo.end(), expo.begin(), expo.end());

                Bits vec, hist;
                for (auto& pe : r.expo) if (pe.second & 1) set_bit(vec, pe.first);
                size_t t = rels.size();
                rels.push_back(r);
                set_bit(hist, t);

                while (true) {
                    int c = lowest_bit(vec);
                    if (c < 0) { 
                        uint64_t d = find_solution(hist, rels, base, n);
                        if (d != 1) return d;
                        for (const Bits& old : solutions) {
                            Bits comb = hist;
                            xor_into(comb, old);
                            d = find_solution(comb, rels, base, n);
                            if (d != 1) return d;
                        }
                        solutions.push_back(hist);
                        break;
                    }
                    if (has_pivot[c]) {
                        xor_into(vec, pivot_vec[c]);
                        xor_into(hist, pivot_hist[c]);
                    } else {
                        has_pivot[c] = true;
                        pivot_vec[c] = vec;
                        pivot_hist[c] = hist;
                        break;
                    }
                }
            }

            uint64_t A_next = (uint64_t)(((unsigned __int128)a * A_cur + A_prev) % n);
            A_prev = A_cur;
            A_cur = A_next;

            if (v == 1) break;  
        }
        if (elapsed() > time_limit) return 1;
    }
    return 1;
}