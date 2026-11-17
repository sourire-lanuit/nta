#include "Header.h"
#include <chrono>
#include <string>

static double run(bool parallel, long long p, long long a, long long b, long long &res) {
    auto s = std::chrono::high_resolution_clock::now();
    res = parallel ? ind_cal_par(p, a, b) : ind_cal_seq(p, a, b);
    std::chrono::duration<double> d = std::chrono::high_resolution_clock::now() - s;
    return d.count();
}

static void result(long long p, long long a, long long b, long long x, double t) {
    std::cout << "\n--- шо ми імеєм ---\n";
    std::cout << "x = " << x << "\n";
    std::cout << "час роботи: " << t << " секунд\n";
}

static void menu() {
    std::cout << "\n--- опції ---\n";
    std::cout << "1. запустити послідовний алгоритм\n";
    std::cout << "2. запустити паралельний алгоритм\n";
    std::cout << "3. n запусків обох алгоритмів, середній час\n";
    std::cout << "0. вийти\n";
    std::cout << "оберіть опцію: ";
}

int main(int argc, char* argv[]) {
    long long p, alpha, beta;
    if (argc >= 4) {
        p = std::stoll(argv[1]); alpha = std::stoll(argv[2]); beta = std::stoll(argv[3]);
        bool par = (argc >= 5 && std::string(argv[4]) == "par");
        long long x;
        double t = run(par, p, alpha, beta, x);
        result(p, alpha, beta, x, t);
        return 0;
    }

    std::cout << "введіть параметри:\n";
    std::cout << "p = ";  
    std::cin >> p;
    std::cout << "alpha = "; 
    std::cin >> alpha;
    std::cout << "beta = ";    
    std::cin >> beta;

    int choice = 0;
    do {
        menu();
        if (!(std::cin >> choice)) break;

        if (choice == 1 || choice == 2) {
            long long x;
            double t = run(choice == 2, p, alpha, beta, x);
            result(p, alpha, beta, x, t);
        } else if (choice == 3) {
            int n; 
            std::cout << "кількість запусків: "; 
            std::cin >> n;
            double ts = 0, tp = 0; bool ok = true;
            for (int i = 0; i < n; i++) {
                long long xs, xp;
                ts += run(false, p, alpha, beta, xs);
                tp += run(true, p, alpha, beta, xp);
                ok = ok && xs == xp && mod_exp(alpha, xs, p) == beta % p;
            }
            std::cout << "середній час послідовного: " << ts / n << " c\n";
            std::cout << "середній час паралельного: " << tp / n << " c\n";
            std::cout << "відповіді збігаються та вірні: " << (ok ? "так" : "ні") << "\n";
        }
    } while (choice != 0);
    return 0;
}