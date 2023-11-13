#ifndef SIPHASH_H
#define SIPHASH_H
#include <stdio.h>
#include <stdint.h>
#include "array.h"

#define C 2
#define D 4

typedef uint64_t uint128_t[2];

uint64_t siphash(const uint128_t k, array(char) *m);
#endif /* SIPHASH_H */