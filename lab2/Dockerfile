# Офіційний образ GCC
FROM gcc:latest

WORKDIR /usr/src/app
COPY *.cpp *.h ./

# Компілюємо програму
RUN g++ -o dlog main.cpp sph.cpp perebir.cpp

# Число передається як аргумент: docker run <образ> <число>
# Без аргументу запускається інтерактивне меню (docker run -it <образ>)
ENTRYPOINT ["./dlog"]