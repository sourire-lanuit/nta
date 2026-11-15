#include "Header.h"
#include <iomanip>

long long g_p[3], g_alpha[3], g_beta[3];
bool g_loaded[3] = {false, false, false};
int g_reps = 7;

double measure(long long (*f)(long long, long long, long long), long long alpha, long long beta, long long p, int reps, long long &result) {
    double total = 0;
    for (int r = 0; r < reps; r++) {
        set_deadline(max_time_sec);
        auto start = std::chrono::steady_clock::now();
        result = f(alpha, beta, p);
        auto end = std::chrono::steady_clock::now();

        if (result == -2) return -1;
        total += std::chrono::duration<double>(end - start).count();
    }
    return total / reps;
}

void print_row(const char* name, double t, long long result) {
    std::cout << name << ": ";
    if (t < 0) {
        std::cout << "перевищено ліміт 5 хвилин\n";
        return;
    }
    std::cout << std::fixed << std::setprecision(6) << t << " с, ";
    if (result == -1) std::cout << "розв'язок не знайдено\n";
    else std::cout << "x = " << result << std::endl << std::endl;
}

void task(int t) {
    std::cout << "\n--- задача " << t << "-го типу ---\n";
    std::cout << "введіть просте число p: ";
    std::cin >> g_p[t];
    std::cout << "введіть генератор alpha: ";
    std::cin >> g_alpha[t];
    std::cout << "введіть елемент beta: ";
    std::cin >> g_beta[t];

    if (std::cin.fail() || g_p[t] < 2) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        g_loaded[t] = false;
        std::cout << "спробуйте ще раз\n";
        return;
    }
    g_loaded[t] = true;
}
void run_task(int t) {
    if (!g_loaded[t]) {
        std::cout << "задачу " << t << "-го типу ще не введено\n";
        return;
    }

    const char* name_perebir = (t == 1) ? "перебір, задача 1" : "перебір, задача 2";
    const char* name_sph = (t == 1) ? "С-П-Г, задача 1" : "С-П-Г, задача 2";

    long long r;
    double time;

    std::cout << "\n--- замір часу, задача " << t << "-го типу ---\n";

    time = measure(perebir, g_alpha[t], g_beta[t], g_p[t], g_reps, r);
    print_row(name_perebir, time, r);
    time = measure(silv_pol_hel, g_alpha[t], g_beta[t], g_p[t], g_reps, r);
    print_row(name_sph, time, r);
}

int main() {
    int choice;

    while (true) {

        std::cout << "1. ввести задачу 1-го типу\n";
        std::cout << "2. ввести задачу 2-го типу\n";
        std::cout << "3. виміряти час для задачі 1-го типу\n";
        std::cout << "4. виміряти час для задачі 2-го типу\n";
        std::cout << "0. вихід\n";
        std::cout << "вибір: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "перевиберіть\n";
            continue;
        }

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            task(1);
        } else if (choice == 2) {
            task(2);
        } else if (choice == 3) {
            run_task(1);
        } else if (choice == 4) {
            run_task(2);
        } else {
            std::cout << "перевиберіть\n";
        }
    }

    return 0;
}