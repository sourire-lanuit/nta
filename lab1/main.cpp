#include "Header.h"
#include <iostream>

int main() {
    int choice;
    uint64_t n;

    while (true) {
        std::cout << "\n--- Методи  факторизації ---\n";
        std::cout << "1. Тест Міллера-Рабіна\n";
        std::cout << "2. Метод пробних ділень\n";
        std::cout << "3. p-метод Полларда\n";
        std::cout << "4. Метод Брілхарта-Моррісона\n";
        std::cout << "5. Канонічний розклад\n";
        std::cout << "0. Вихід\n";
        std::cout << "Обраний метод: ";
        std::cin >> choice;

        if (choice == 0) break;

        std::cout << "Введіть число: ";
        std::cin >> n;

        switch (choice) {
            case 1:
                std::cout << (miller_rabin(n) ? "Число просте" : "Число складене") << "\n";
                break;
            case 2:
                std::cout << "Дільник: " << trial_division(n) << "\n";
                break;
            case 3:
                std::cout << "Дільник: " << pollard(n) << "\n";
                break;
            case 4:
                std::cout << "Дільник: " << brillhart_morrison(n) << "\n";
                break;
            case 5:
                canonical(n);
                break;
            default:
                std::cout << "Переоберіть\n";
        }
    }
    return 0;
}