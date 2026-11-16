#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <map>

long long mod_inv(long long a, long long m);
long long ext_gcd(long long a, long long b, long long &x, long long &y);
long long mod_exp(long long base, long long exp, long long mod);
std::vector<int> factor_base(int B);

long long ind_cal_seq(long long p, long long alpha, long long beta);
long long ind_cal_par(long long p, long long alpha, long long beta);

#endif