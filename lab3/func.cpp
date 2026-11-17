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

long long else_mod(long long a, long long b, long long m) {
    a %= m; b %= m;
    if (a < 0) a += m;
    if (b < 0) b += m;
    #ifdef __sizeof_int128__
    return (long long)((__int128)a * b % m);
    #else 
    long long r = 0;
    while (b) {
        if (b & 1) r = (r + a) % m;
        a = (a << 1) % m;
        b >>= 1;
    }
    return r;
    #endif
}

std::vector<int> factor_base(int B) {
    std::vector<int> primes;
    if (B < 2) return primes;
    std::vector<bool> is_prime(B + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (long long p = 2; p * p <= B; p++)
        if (is_prime[p])
            for (long long i = p * p; i <= B; i += p) is_prime[i] = false;
    for (int p = 2; p <= B; p++)
        if (is_prime[p]) primes.push_back(p);
    return primes;
}

int compute_bound(long long p) {
    double n = (double)(p - 1);
    double c = 3.38;
    double B = 5;
    if (n > 16) {
        double ln = std::log(n);
        B = c * std::exp(0.5 * std::sqrt(ln * std::log(ln)));
    }
    B = std::max(5.0, B);
    B = std::min(B, (double)(p - 1)); 
    return (int)B;
}

bool factor_over_base(long long val, const std::vector<int>& fb, std::vector<int>& exps) {
    for (size_t i = 0; i < fb.size() && val > 1; i++) {
        while (val % fb[i] == 0) { exps[i]++; val /= fb[i]; }
    }
    return val == 1;
}

static std::vector<std::pair<long long, int>> factorize(long long n) {
    std::vector<std::pair<long long, int>> f;
    for (long long q = 2; q * q <= n; q++) {
        if (n % q == 0) {
            int e = 0;
            while (n % q == 0) { n /= q; e++; }
            f.push_back({q, e});
        }
    }
    if (n > 1) f.push_back({n, 1});
    return f;
}

std::vector<long long> solve_logs(const std::vector<Relation>& rels, int t, long long n) {
    auto fac = factorize(n);
    int m = (int)rels.size();
    std::vector<long long> mods, sols_all;
    std::vector<std::vector<long long>> parts;

    for (auto& qe : fac) {
        long long q = qe.first, mod = 1;
        for (int i = 0; i < qe.second; i++) mod *= q;

        std::vector<std::vector<long long>> A(m, std::vector<long long>(t + 1));
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < t; c++) A[r][c] = rels[r].expo[c] % mod;
            A[r][t] = rels[r].k % mod;
        }
        int row = 0;
        for (int c = 0; c < t; c++) {
            int piv = -1;
            for (int r = row; r < m; r++)
                if (A[r][c] % q != 0) { piv = r; break; }
            if (piv < 0) return {}; 
            std::swap(A[row], A[piv]);
            long long inv = mod_inv(A[row][c], mod);
            for (int j = 0; j <= t; j++) A[row][j] = else_mod(A[row][j], inv, mod);
            for (int r = 0; r < m; r++) {
                if (r == row) continue;
                long long f = A[r][c];
                if (!f) continue;
                for (int j = 0; j <= t; j++)
                    A[r][j] = ((A[r][j] - else_mod(f, A[row][j], mod)) % mod + mod) % mod;
            }
            row++;
        }
        std::vector<long long> s(t);
        for (int c = 0; c < t; c++) s[c] = A[c][t];
        mods.push_back(mod);
        parts.push_back(s);
    }

    std::vector<long long> logs(t, 0);
    for (size_t i = 0; i < mods.size(); i++) {
        long long Mi = n / mods[i];
        long long inv = mod_inv(Mi % mods[i], mods[i]);
        for (int c = 0; c < t; c++) {
            long long term = else_mod(else_mod(parts[i][c], inv, n), Mi, n);
            logs[c] = (logs[c] + term) % n;
        }
    }
    return logs;
}

long long finalize(long long p, long long alpha, long long beta, const std::vector<int>& fb, const std::vector<long long>& logs) {
    long long n = p - 1;
    int t = (int)fb.size();
    std::random_device rd;
    std::seed_seq seq{rd(), rd(), (unsigned)std::random_device{}()};
    std::mt19937_64 gen(seq);
    std::uniform_int_distribution<long long> dis(0, n - 1);

    while (true) {
        long long l = dis(gen);
        long long val = else_mod(beta, mod_exp(alpha, l, p), p);
        std::vector<int> exps(t, 0);
        if (factor_over_base(val, fb, exps)) {
            long long x = (n - l % n) % n;
            for (int i = 0; i < t; i++)
                x = (x + else_mod(exps[i], logs[i], n)) % n;
            return x;
        }
    }
}