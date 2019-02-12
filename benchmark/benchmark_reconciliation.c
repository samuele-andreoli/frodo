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
    FRODO_CSPRNG RNG;
    char seed[100];

    for (int i=0; i<100; i++)
        seed[i] = i;
    RAND_seed(&RNG,100,seed);

    printf("Benchmark reconciliation\n\n");

    // Instantiate recombined shares
    uint16_t m[FRODO_BAR_N][FRODO_BAR_N] = {0};
    for(int i = 0; i < FRODO_BAR_N; i++)
    {
        for(int j = 0; j < FRODO_BAR_N; j++)
        {
            m[i][j] = (uint8_t)RAND_byte(&RNG) << 8 | (uint8_t)RAND_byte(&RNG);
        }
    }

    // Regular econciliation
    uint8_t key[FRODO_KEY_LENGTH] = {0};
    FRODO_reconcilitaion(key, (uint16_t*)m);

    BENCHTEST("plain reconciliation", FRODO_reconcilitaion(key, (uint16_t*)m), 10000000);

    // Reconciliation with hint
    uint8_t hint[FRODO_HINT_LENGTH] = {0};
    uint8_t key_with_hint[FRODO_KEY_LENGTH] = {0};

    BENCHTEST("hint computation", FRODO_hint(hint, (uint16_t*)m), 10000000);
    BENCHTEST("reconciliation with hint", FRODO_reconcilitaion_with_hint(key_with_hint, hint, (uint16_t*)m), 10000000);

    return 0;
}