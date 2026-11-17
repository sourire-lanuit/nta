#include "Header.h"
#include <thread>
#include <mutex>
#include <chrono>

long long ind_cal_par(long long p, long long alpha, long long beta) {
    long long n = p - 1;
    alpha %= p; beta %= p;
    if (n < 2 || alpha == 0 || beta == 0) return -1;

    std::vector<int> fb = factor_base(compute_bound(p));
    int t = (int)fb.size();
    if (t == 0) return -1;

    unsigned num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4;

    std::vector<Relation> relations;
    std::mutex rm;

    auto collect = [&](size_t target) {
        auto worker = [&](unsigned id) {
            std::random_device rd;
            std::seed_seq seq{rd(), (unsigned)id, (unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count()};
            std::mt19937_64 gen(seq);
            std::uniform_int_distribution<long long> dis(1, n - 1);
            std::vector<int> exps(t);

            while (true) {
                {
                    std::lock_guard<std::mutex> lock(rm);
                    if (relations.size() >= target) return;
                }
                long long k = dis(gen);
                std::fill(exps.begin(), exps.end(), 0);
                if (factor_over_base(mod_exp(alpha, k, p), fb, exps)) {
                    std::lock_guard<std::mutex> lock(rm);
                    if (relations.size() < target) relations.push_back({k, exps});
                }
            }
        };
        std::vector<std::thread> threads;
        for (unsigned i = 0; i < num_threads; i++) threads.emplace_back(worker, i);
        for (auto& th : threads) th.join();
    };

    std::vector<long long> logs;
    size_t target = t + 5;
    while (true) {
        collect(target);
        logs = solve_logs(relations, t, n);
        if (!logs.empty()) break;
        target += t / 2 + 5;
    }
    return finalize(p, alpha, beta, fb, logs);
}