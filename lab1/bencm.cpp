#include "Header.h"

static double ms_since(std::chrono::steady_clock::time_point t0) {
    return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
}

void benchmark() {
    const uint64_t nums[] = {
        3009182572376191ULL, 1021514194991569ULL, 4000852962116741ULL, 15196946347083ULL, 499664789704823ULL, 
        269322119833303ULL, 679321846483919ULL, 96267366284849ULL, 61333127792637ULL, 2485021628404193ULL
    };

    std::cout << "\n--- Порівняння р-методу Полларда та методу Брілхарта-Моррісона ---\n";
    std::cout << std::left << std::setw(20) << "Число" << std::setw(18) << "  Поллард, дільник" << std::setw(14) << "     Поллард, мс" << std::setw(18) << "    БМ, дільник" << std::setw(14) << "       БМ, мс" << "\n";
    for (uint64_t n : nums) {
        auto t0 = std::chrono::steady_clock::now();
        uint64_t d1 = pollard(n);
        double t1 = ms_since(t0);
        t0 = std::chrono::steady_clock::now();
        uint64_t d2 = brillhart_morrison(n, 30.0);
        double t2 = ms_since(t0);
        std::cout << std::left << std::setw(20) << n << std::setw(18) << d1 << std::setw(14) << t1 << std::setw(18) << d2 << std::setw(14) << t2 << "\n";
    }

    std::cout << "\n--- Тест Міллера-Рабіна, середній час на 1000 запусків ---\n";
    const uint64_t primes[] = { 1000000007ULL, 1000000000039ULL, 1000000000000037ULL, 1000000000000000003ULL, 
                                2305843009213693951ULL, 18446744073709551557ULL
    };

    for (uint64_t p : primes) {
        auto t0 = std::chrono::steady_clock::now();
        bool r = false;
        for (int i = 0; i < 1000; ++i) r = miller_rabin(p);
        std::cout << std::left << std::setw(24) << p << (r ? "просте   " : "складене ") << ms_since(t0) / 1000.0 << " мс\n";
    }
}