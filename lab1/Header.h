#ifndef HEADER_H
#define HEADER_H

#include <cstdint>
#include <vector>
#include <string>
#include <map>

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m);
uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod);
uint64_t gcd(uint64_t a, uint64_t b);

bool miller_rabin(uint64_t n, int k = 10);

uint64_t trial_division(uint64_t n);

uint64_t pollard(uint64_t n);

uint64_t brillhart_morrison(uint64_t n);
uint64_t is_sqrt(uint64_t n);
std::vector<int> factor_base(uint64_t n, int bound, int k = 10);
std::vector<uint64_t> fraction_sqrt(uint64_t n, int limit = 200);
bool b_smooth(long long val, const std::vector<int>& base, std::map<int, int>& factors);

#endif 