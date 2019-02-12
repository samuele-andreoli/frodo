/*
 * ISC License
 *
 * Copyright (c) 2019
 * Samuele Andreoli <samuele dot andreoli at studenti dot unitn dot it>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "benchmark.h"

int main() {
    printf("Benchmark parameter and key generation\n\n");

    // Seed RNG
    FRODO_CSPRNG RNG;
    char rng_seed[100];

    for (int i=0; i<100; i++)
        rng_seed[i] = i;
    RAND_seed(&RNG, 100, rng_seed);

    // Generate seed
    uint8_t param_seed[FRODO_SEED_LENGTH] = {0};
    for(int i=0; i<FRODO_SEED_LENGTH; i++)
    {
        param_seed[i] = (uint8_t)RAND_byte(&RNG);
    }

    // Generate secrets
    uint16_t sl[FRODO_BAR_N][FRODO_N];
    for(int i=0; i<FRODO_BAR_N * FRODO_N; i++)
    {
        ((uint16_t*)sl)[i]= ((uint16_t)RAND_byte(&RNG) << 8) | ((uint16_t)RAND_byte(&RNG));
    }

    uint16_t sr[FRODO_N][FRODO_BAR_N];
    for(int i=0; i<FRODO_N * FRODO_BAR_N; i++)
    {
        ((uint16_t*)sr)[i]=((uint16_t)RAND_byte(&RNG) << 8) | ((uint16_t)RAND_byte(&RNG));
    }

    // Generate parameter
    uint16_t a[FRODO_N][FRODO_N] = {0};

    BENCHTEST("generate parameter", FRODO_generate_a(a, param_seed), 100);

    // Generation on the fly
    uint16_t as_auto[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t sa_auto[FRODO_BAR_N][FRODO_N] = {0};

    BENCHTEST("generate and multiply by row", FRODO_generate_multiply_by_row(as_auto, sr, param_seed), 100);
    BENCHTEST("generate and multiply by column", FRODO_generate_multiply_by_column(sa_auto, sl, param_seed), 100);

    // Generation from parameter
    uint16_t as_man[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t sa_man[FRODO_BAR_N][FRODO_N] = {0};

    BENCHTEST("multiply by row", FRODO_parameter_right_mul(as_man, sr, a), 100);
    BENCHTEST(" multiply by column", FRODO_parameter_left_mul(sa_man, sl, a), 100);

    return 0;
}