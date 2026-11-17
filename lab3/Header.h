#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <map>
#include <algorithm>

struct Relation {
    long long k;
    std::vector<int> expo;
};

long long mod_inv(long long a, long long m);
long long ext_gcd(long long a, long long b, long long &x, long long &y);
long long mod_exp(long long base, long long exp, long long mod);
std::vector<int> factor_base(int B);
long long else_mod(long long base, long long exp, long long mod);
int  compute_bound(long long p);
bool factor_over_base(long long val, const std::vector<int>& fb, std::vector<int>& exps);
std::vector<long long> solve_logs(const std::vector<Relation>& rels, int t, long long n);
long long finalize(long long p, long long alpha, long long beta, const std::vector<int>& fb, const std::vector<long long>& logs); 

long long ind_cal_seq(long long p, long long alpha, long long beta);
long long ind_cal_par(long long p, long long alpha, long long beta);

#endif