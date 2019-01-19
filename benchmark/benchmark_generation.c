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

#include <time.h>
#include "frodo.h"

float start, end;

#define BENCHTEST(name, func, iterations) \
	printf("------------------------------------------------------------\n"); \
	printf("Benchmark %s\n", name); \
	start = (float)clock(); \
	for(int i = 0; i < iterations; i++) \
	{ \
		func; \
	} \
	end = (float)clock(); \
	printf("Time elapsed %f\n", (end-start)/CLOCKS_PER_SEC); \
	printf("Iterations %d\n", iterations); \
	printf("Time per iteration %fs\n", (end-start)/(CLOCKS_PER_SEC * iterations)); \
	printf("------------------------------------------------------------\n");

int main() {
	csprng RNG;
    char seed[100];

    for (int i=0;i<100;i++)
        seed[i] = i;
    RAND_seed(&RNG,100,seed);

    printf("Benchmark parameter and key generation\n\n");

    // Generate seed
    uint8_t param_seed[FRODO_SEED_LENGTH] = {0};

    // Generate parameter
    uint16_t a[FRODO_N][FRODO_N] = {0};

    BENCHTEST("generate parameter", FRODO_generate_a(a, param_seed), 100)

    // Generate keys from seed
    FRODO_left_keypair lk = {0};
    FRODO_right_keypair rk = {0};

    BENCHTEST("generate left keypair from seed", FRODO_generate_left_keypair(&RNG, &lk, param_seed), 100);
    BENCHTEST("generate right keypair from seed", FRODO_generate_right_keypair(&RNG, &rk, param_seed), 100);

    // Generate keys from full parameter
    FRODO_left_keypair lk_full = {0};
    FRODO_right_keypair rk_full = {0};

    BENCHTEST("generate left keypair from parameter", FRODO_generate_left_keypair_full_param(&RNG, &lk_full, a), 100);
    BENCHTEST("generate right keypair from parameter", FRODO_generate_right_keypair_full_param(&RNG, &rk_full, a), 100);
}