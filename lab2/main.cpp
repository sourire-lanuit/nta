#include "Header.h"

int main() {
    int choice;
    long long alpha, beta, p;

    std::cout << "1. метод звичайного перебору\n";
    std::cout << "2. алгоритм Сільвера-Поліга-Геллмана\n";
    std::cout << "вибір: ";
    std::cin >> choice;

    if (choice != 1 && choice != 2) {
        std::cout << "перевиберіть\n";
        return 1;
    }

    std::cout << "введіть просте число p: ";
    std::cin >> p;
    std::cout << "введіть генератор alpha: ";
    std::cin >> alpha;
    std::cout << "введіть елемент beta: ";
    std::cin >> beta;

    long long result = -1;

    if (choice == 1) {
        result = perebir(alpha, beta, p);
    } else {
        result = silv_pol_hel(alpha, beta, p);
    }

    if (result != -1) {
        std::cout << "результат: x = " << result << std::endl;
    } else {
        std::cout << "пу-пу-пу...\n";
    }

    return 0;
}