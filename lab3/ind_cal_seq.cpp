#include "Header.h"

long long ind_cal_seq(long long p, long long alpha, long long beta) {
    long long n = p - 1;
    alpha %= p; beta %= p;
    if (n < 2 || alpha == 0 || beta == 0) return -1;

    std::vector<int> fb = factor_base(compute_bound(p));
    int t = (int)fb.size();
    if (t == 0) return -1;

    std::random_device rd;
    std::seed_seq seq{rd(), rd(), (unsigned)std::random_device{}()};
    std::mt19937_64 gen(seq);
    std::uniform_int_distribution<long long> dis(1, n - 1);

    std::vector<Relation> relations;
    std::vector<long long> logs;
    size_t target = t + 5;

    while (true) {
        while (relations.size() < target) {
            long long k = dis(gen);
            std::vector<int> exps(t, 0);
            if (factor_over_base(mod_exp(alpha, k, p), fb, exps))
                relations.push_back({k, exps});
        }
        logs = solve_logs(relations, t, n);
        if (!logs.empty()) break;
        target += t / 2 + 5;
    }
    return finalize(p, alpha, beta, fb, logs);
}