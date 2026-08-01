# Використовуємо офіційний образ GCC
FROM gcc:latest

# Встановлюємо робочу директорію в контейнері
WORKDIR /usr/src/app

# Копіюємо всі файли з поточної директорії хоста до робочої директорії контейнера
COPY . .

# Компілюємо програму
RUN g++ -O3 -o factorization main.cpp func.cpp milrab.cpp trialdiv.cpp pollard.cpp brillmor.cpp canonical.cpp

# Вказуємо команду, яка буде виконана при запуску контейнера
CMD ["./factorization"]