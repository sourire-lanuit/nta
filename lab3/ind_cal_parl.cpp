#include "Header.h"
#include <thread>
#include <mutex>
#include <atomic>

struct Relation {
    long long k;
    std::vector<int> expo;
};

long long ind_cal_par(long long p, long long alpha, long long beta) {
    long long n = p - 1;
    double c_const = 3.38;
    int B = std::max(5.0, exp(c_const * sqrt(log(n) * log(log(n))))); 
    
    std::vector<int> factorbase = factor_base(B);
    int t = factorbase.size();
    if (t == 0) return -1;

    std::vector<Relation> relations;
    std::mutex rm;
    int tr = t + 5;
    
    auto worker = [&]() {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<long long> dis(1, n - 1);
        
        while (true) {
            rm.lock();
            if (relations.size() >= tr) {
                rm.unlock();
                break;
            }
            rm.unlock();

            long long k = dis(gen);
            long long val = mod_exp(alpha, k, p);
            std::vector<int> exps(t, 0);

            for (int i = 0; i < t; i++) {
                while (val % factorbase[i] == 0) {
                    exps[i]++;
                    val /= factorbase[i];
                }
            }

            if (val == 1) {
                std::lock_guard<std::mutex> lock(rm);
                if (relations.size() < tr) {
                    relations.push_back({k, exps});
                }
            }
        }
    };

    int num_threads = std::thread::hardware_concurrency();
    if(num_threads == 0) num_threads = 4;
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(worker);
    }
    for (auto &th : threads) {
        th.join();
    }

    std::vector<long long> logs(t, 0); 

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<long long> dis(1, n - 1);

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