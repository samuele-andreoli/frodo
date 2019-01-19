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

#include "amcl.h"
#include "lwe_generate.h"

int main()
{
    csprng RNG;
    char rng_seed[100];

    uint16_t sl[FRODO_BAR_N][FRODO_N];
    uint16_t sr[FRODO_N][FRODO_BAR_N];
    uint16_t a[FRODO_N][FRODO_N] = {0};
    uint16_t as_auto[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t as_man[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t sa_auto[FRODO_BAR_N][FRODO_N] = {0};
    uint16_t sa_man[FRODO_BAR_N][FRODO_N] = {0};

    for (int i=0;i<100;i++)
        rng_seed[i] = i;
    RAND_seed(&RNG, 100, rng_seed);

    uint8_t seed[FRODO_SEED_LENGTH];
    for(int i=0;i<FRODO_SEED_LENGTH;i++)
    {
        seed[i] = (uint8_t)RAND_byte(&RNG);
    }

    for(int i=0;i<FRODO_BAR_N * FRODO_N;i++)
    {
        ((uint16_t*)sl)[i]= ((uint16_t)RAND_byte(&RNG) << 8) | ((uint16_t)RAND_byte(&RNG));
    }
    for(int i=0;i<FRODO_N * FRODO_BAR_N;i++)
    {
        ((uint16_t*)sr)[i]=((uint16_t)RAND_byte(&RNG) << 8) | ((uint16_t)RAND_byte(&RNG));
    }

    FRODO_generate_a(a, seed);
    FRODO_generate_multiply_by_column(sa_auto, sl, seed);
    FRODO_generate_multiply_by_row(as_auto, sr, seed);
    FRODO_parameter_left_mul(sa_man, sl, a);
    FRODO_parameter_right_mul(as_man, sr, a);

    uint16_t* pauto = (uint16_t*)sa_auto;
    uint16_t* pman = (uint16_t*)sa_man;
    for(int i=0;i<FRODO_BAR_N * FRODO_N;i++)
    {
        if(pauto[i] != pman[i])
        {
            printf("Inconsistency in left share\n");
            return 1;
        }
    }

    pauto = (uint16_t*)as_auto;
    pman = (uint16_t*)as_man;
    for(int i=0;i<FRODO_N * FRODO_BAR_N;i++)
    {
        if(pauto[i] != pman[i])
        {
            printf("Inconsistency in right share\n");
            return 1;
        }
    }

    return 0;
}