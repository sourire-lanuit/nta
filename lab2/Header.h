#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <chrono>

constexpr double max_time_sec = 300.0;
void set_deadline(int seconds);
bool timed_out();

long long power_mod(long long base, long long exp, long long mod);
long long perebir(long long alpha, long long beta, long long p);

long long ext_gcd(long long a, long long b, long long &x, long long &y);
long long mod_inv(long long a, long long m);
long long silv_pol_hel(long long alpha, long long beta, long long p);

#endif