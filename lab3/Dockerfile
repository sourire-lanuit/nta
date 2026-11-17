FROM gcc:13 AS build
WORKDIR /src
COPY *.cpp Header.h ./
RUN g++ -O2 -std=c++17 -pthread -o indcalc main.cpp func.cpp ind_cal_seq.cpp ind_cal_parl.cpp

FROM debian:stable-slim
COPY --from=build /src/indcalc /usr/local/bin/indcalc
# Запуск: docker run --rm indcalc <p> <alpha> <beta>   (додайте "par" для паралельної версії)
ENTRYPOINT ["indcalc"]