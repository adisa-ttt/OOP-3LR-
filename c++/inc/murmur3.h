#pragma once
#include <cstdint>

inline uint32_t Fmix(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

inline uint32_t murmur3_32(uint32_t key, uint32_t seed) {
    const uint32_t c1 = 0xcc9e2d51;
    //const uint32_t c2 = 0x1b873593;

    uint32_t k = key;
    uint32_t h = seed;
    k *= c1;

    k = (k << 15) | (k >> 17);
    h ^= k;
    h = (h << 13) | (h >> 19);
    h = h*5 + 0xe6546b64;

    h ^= 4;
    h = Fmix(h);

    return h;
}