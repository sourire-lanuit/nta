#include "Header.h"

struct Relation {
    long long k;
    std::vector<int> expo;
};

long long ind_cal_seq(long long p, long long alpha, long long beta) {
    long long n = p - 1;
    double c_const = 3.38;
    int B = std::max(5.0, exp(c_const * sqrt(log(n) * log(log(n))))); 
    
    std::vector<int> factorbase = factor_base(B);
    int t = factorbase.size();
    if (t == 0) return -1;

    std::vector<Relation> relations;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dis(1, n - 1);

    while (relations.size() < t + 5) {
        long long k = dis(gen);
        long long val = mod_exp(alpha, k, p);
        std::vector<int> exps(t, 0);
        bool is_smooth = true;

        for (int i = 0; i < t; i++) {
            while (val % factorbase[i] == 0) {
                exps[i]++;
                val /= factorbase[i];
            }
        }

        if (val == 1) {
            relations.push_back({k, exps});
        }
    }

    std::vector<long long> logs(t, 0);
    while (true) {
        long long l = dis(gen);
        long long val = (beta * mod_exp(alpha, l, p)) % p;
        std::vector<int> exps(t, 0);
        
        for (int i = 0; i < t; i++) {
            while (val % factorbase[i] == 0) {
                exps[i]++;
                val /= factorbase[i];
            }
        }

        if (val == 1) {
            long long logBeta = -l;
            for (int i = 0; i < t; i++) {
                logBeta = (logBeta + exps[i] * logs[i]) % n;
            }
            return (logBeta + n) % n;
        }
    }
}