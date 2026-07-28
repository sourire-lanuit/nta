#ifndef HEADER_H
#define HEADER_H

#include <cstdint>
#include <vector>
#include <string>

uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m);
uint64_t power_mod(uint64_t base, uint64_t exp, uint64_t mod);
uint64_t gcd(uint64_t a, uint64_t b);

bool miller_rabin(uint64_t n, int k = 10);

#endif 