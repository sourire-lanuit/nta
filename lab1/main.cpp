#include "Header.h"
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

static bool parse_number(const std::string& s, uint64_t& n) {
    if (s.empty() || s.find_first_not_of("0123456789") != std::string::npos) return false;
    try { n = std::stoull(s); } catch (...) { return false; }
    return true;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    if (argc > 1) {
        uint64_t n;
        if (!parse_number(argv[1], n)) {
            std::cerr << "Помилка: число має бути натуральним\n";
            return 1;
        }
        return canonical(n) ? 0 : 2;
    }

    while (true) {
        std::cout << "\n--- Методи факторизації ---\n"
                  << "1. Тест Міллера-Рабіна\n"
                  << "2. Метод пробних ділень\n"
                  << "3. р-метод Полларда\n"
                  << "4. Метод Брілхарта-Моррісона\n"
                  << "5. Канонічний розклад\n"
                  << "6. Порівняння Полларда та Брілхарта-Моррісона\n"
                  << "0. Вихід\n"
                  << "Обраний метод: ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        int choice;
        try { choice = std::stoi(line); } catch (...) { std::cout << "Переоберіть\n"; continue; }

        if (choice == 0) break;
        if (choice == 6) { benchmark(); continue; }
        if (choice < 1 || choice > 5) { std::cout << "Переоберіть\n"; continue; }

        std::cout << "Введіть число: ";
        std::getline(std::cin, line);
        uint64_t n;
        if (!parse_number(line, n) || n < 2) {
            std::cout << "Потрібне натуральне число n >= 2\n";
            continue;
        }

        uint64_t d;
        switch (choice) {
            case 1:
                std::cout << (miller_rabin(n) ? "Число просте" : "Число складене") << "\n";
                break;
            case 2:
                d = trial_division(n);
                if (d == 1) std::cout << "Дільників серед простих чисел <= 47 немає\n";
                else std::cout << "Дільник: " << d << "\n";
                break;
            case 3:
                d = pollard(n);
                if (d == 1) std::cout << "Дільник не знайдено\n";
                else std::cout << "Дільник: " << d << "\n";
                break;
            case 4:
                d = brillhart_morrison(n);
                if (d == 1) std::cout << "Дільник не знайдено\n";
                else std::cout << "Дільник: " << d << "\n";
                break;
            case 5:
                canonical(n);
                break;
        }
    }
    return 0;
}