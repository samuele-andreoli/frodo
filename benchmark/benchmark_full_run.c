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
	csprng RNG;
    char seed[100];

    for (int i=0;i<100;i++)
        seed[i] = i;
    RAND_seed(&RNG,100,seed);

    printf("Benchmark protocol run\n\n");
    FRODO_left_keypair lk = {0};
    FRODO_right_keypair rk = {0};

    // Generate seed
    uint8_t param_seed[FRODO_SEED_LENGTH] = {0};

    // Generate keys
    BENCHTEST("generate left keypair", FRODO_generate_left_keypair(&RNG, &lk, param_seed), 100);
    BENCHTEST("generate right keypair", FRODO_generate_right_keypair(&RNG, &rk, param_seed), 100);

    // Pack right key
    uint8_t right_keyshare[FRODO_PACKED_SHARE_LENGTH] = {0};

    BENCHTEST("pack right keyshare", FRODO_right_keyshare_pack(right_keyshare, &rk), 10000);

    // Perform left keyshare
    uint8_t key_left[FRODO_KEY_LENGTH] = {0};
    uint8_t left_keyshare[FRODO_PACKED_SHARE_LENGTH] = {0};
    uint8_t hint[FRODO_HINT_LENGTH] = {0};

    BENCHTEST("left key agreement", FRODO_left_key_agreement(&RNG, key_left, left_keyshare, hint, right_keyshare, &lk), 10000);

    // Perform right keyshare
    uint8_t key_right[FRODO_KEY_LENGTH] = {0};

    BENCHTEST("right key agreement", FRODO_right_key_agreement(key_right, left_keyshare, &rk, hint), 10000);
}