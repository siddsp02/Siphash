#include <stdio.h>
#include <stdint.h>
#include "array.h"

#define C 2
#define D 4

typedef uint64_t uint128_t[2];

#define ROTL64(n, b) (((n) << (b)) | (n) >> (64 - (b)))

#define sipround(v0, v1, v2, v3) do {   \
    v0 += v1;                           \
    v1 = ROTL64(v1, 13) ^ v0;           \
    v0 = ROTL64(v0, 32);                \
    v2 += v3;                           \
    v3 = ROTL64(v3, 16) ^ v2;           \
    v2 += v1;                           \
    v1 = ROTL64(v1, 17) ^ v2;           \
    v2 = ROTL64(v2, 32);                \
    v0 += v3;                           \
    v3 = ROTL64(v3, 21) ^ v0;           \
} while (0)

uint64_t siphash(uint128_t k, array(char) *m) {
    uint64_t k0, k1, m_i, v0, v1, v2, v3;
    size_t b, i, j, w;
    k0 = k[0];
    k1 = k[1];
    v0 = k0 ^ 0x736F6D6570736575;
    v1 = k1 ^ 0x646F72616E646F6D;
    v2 = k0 ^ 0x6C7967656E657261;
    v3 = k1 ^ 0x7465646279746573;
    b = len((*m));
    w = (b + 7) / 8;
    for (i = 0; i < ((w * 8) - b); ++i)
        arr_push((*m), '\0');
    for (i = 0; i < w; ++i) {
        m_i = ((uint64_t *) (*m))[i];
        if (i == w - 1)
            m_i |= (b % 256) << 56;
        v3 ^= m_i;
        for (j = 0; j < C; ++j)
            sipround(v0, v1, v2, v3);
        v0 ^= m_i;
    }
    v2 ^= 0xFF;
    for (i = 0; i < D; ++i)
        sipround(v0, v1, v2, v3);
    return v0 ^ v1 ^ v2 ^ v3;
}

int main() {
    uint128_t k = {0x0706050403020100, 0x0F0E0D0C0B0A0908};
    array(char) m = arr_new(char);
    for (size_t i = 0; i < 15; ++i)
        arr_push(m, i);
    uint64_t result = siphash(k, &m);
    printf("%llx\n", result);
    arr_dest(m);
    return 0;
}
