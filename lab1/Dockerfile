# Офіційний образ GCC
FROM gcc:latest

WORKDIR /usr/src/app
COPY *.cpp *.h ./

# Компілюємо програму
RUN g++ -O2 -o factorization main.cpp func.cpp milrab.cpp trialdiv.cpp pollard.cpp brillmor.cpp canonical.cpp bencm.cpp

# Число передається як аргумент: docker run <образ> <число>
# Без аргументу запускається інтерактивне меню (docker run -it <образ>)
ENTRYPOINT ["./factorization"]