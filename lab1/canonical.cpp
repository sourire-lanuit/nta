#include "Header.h"
#include <ctime>
#include <sstream>

std::string timestamp() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm_{};
#ifdef _WIN32
    localtime_s(&tm_, &t);
#else
    localtime_r(&t, &tm_);
#endif
    std::ostringstream os;
    os << std::put_time(&tm_, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0') << ms;
    return os.str();
}

void print_factor(uint64_t factor, const std::string& alg_name) {
    std::cout << "Знайдено дільник: " << factor << "        | Алгоритм: " << alg_name << "       | Час: " << timestamp() << "\n";
}


static bool factorize(uint64_t n, std::vector<uint64_t>& res) {
    if (n == 1) return true;
    if (miller_rabin(n)) {
        print_factor(n, "Тест Міллера-Рабіна");
        res.push_back(n);
        return true;
    }

    uint64_t d = trial_division(n);
    if (d != 1 && d != n) {
        print_factor(d, "Метод пробних ділень");
        res.push_back(d);
        return factorize(n / d, res);
    }

    d = pollard(n);
    if (d != 1 && d != n) {
        print_factor(d, "р-метод Полларда");
        return factorize(d, res) && factorize(n / d, res);
    }

    d = brillhart_morrison(n);
    if (d != 1 && d != n) {
        print_factor(d, "Метод Брілхарта-Моррісона");
        return factorize(d, res) && factorize(n / d, res);
    }

    return false;
}

bool canonical(uint64_t n) {
    if (n < 2) {
        std::cout << "Канонічний розклад визначено для n >= 2\n";
        return false;
    }
    auto t0 = std::chrono::steady_clock::now();
    std::cout << "\nПочаток роботи алгоритму. Час: " << timestamp() << "\n";
    std::cout << "Пошук канонічного розкладу числа: " << n << "\n";

    std::vector<uint64_t> factors;
    bool ok = factorize(n, factors);

    std::cout << "Кінець роботи алгоритму. Час: " << timestamp() << "\n";
    double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    std::cout << "Всього витрачено часу: " << ms << " мс\n";

    if (!ok) {
        std::cout << "канонічний розклад не знайдено\n";
        return false;
    }

    std::sort(factors.begin(), factors.end());
    std::cout << "Канонічний розклад: " << n << " = ";
    for (size_t i = 0; i < factors.size();) {
        size_t j = i;
        while (j < factors.size() && factors[j] == factors[i]) ++j;
        if (i) std::cout << " * ";
        std::cout << factors[i];
        if (j - i > 1) std::cout << "^" << (j - i);
        i = j;
    }
    std::cout << "\n";
    return true;
}