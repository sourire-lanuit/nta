#include "Header.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>


void print_factor(uint64_t factor, const std::string& alg_name) {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::cout << " Знайдено дільник: " << factor << " | Алгоритм: " << alg_name << " | Час: " << ms << " мс\n";
}

void canonical(uint64_t n) {
    auto start = std::chrono::system_clock::now();
    auto start_ms = std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch()).count();
    time_t start_time = std::chrono::system_clock::to_time_t(start);
    std::cout << "\nПочаток роботи алгоритму. Час: " << start_ms << " мс\n";
    std::cout << "Пошук канонічного розкладу числа: " << n << "\n";

    std::vector<uint64_t> factors;
    
    auto process = [&](auto self, uint64_t current_n) -> void {
        if (current_n == 1) return;
        if (miller_rabin(current_n)) {
            factors.push_back(current_n);
            print_factor(current_n, "Міллер-Рабін: ");
            return;
        }

        uint64_t factor = trial_division(current_n);
        if (factor != 1) {
            factors.push_back(factor);
            print_factor(factor, "Пробні ділення: ");
            self(self, current_n / factor);
            return;
        }

        factor = pollard(current_n);
        if (factor != 1 && factor != current_n) {
            factors.push_back(factor);
            print_factor(factor, "р-метод Полларда: ");
            self(self, current_n / factor);
            return;
        }

        factor = brillhart_morrison(current_n);
        if (factor != 1 && factor != current_n) {
            factors.push_back(factor);
            print_factor(factor, "Брілхарт-Моррісон: ");
            self(self, current_n / factor);
            return;
        }

        std::cout << " число не знайдено\n";
    };

    process(process, n);

    auto end = std::chrono::system_clock::now();
    auto end_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end.time_since_epoch()).count();
    time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "this is the end. Час: " << end_ms << " мс\n";
    std::cout << "Всього витрачено часу: " << end_ms - start_ms << " мс\n";
    
    std::cout << "Канонічний розклад: ";
    for (size_t i = 0; i < factors.size(); ++i) {
        std::cout << factors[i] << (i == factors.size() - 1 ? "" : " * ");
    }
    std::cout << "\n";
}