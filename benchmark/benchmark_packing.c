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

// Buffer for time measurements
float times[1000];

int main() {
    FRODO_CSPRNG RNG;
    char seed[100];

    // Non random seed for testing purposes
    for (int i=0; i<100; i++)
        seed[i] = i;
    RAND_seed(&RNG,100,seed);

    printf("Benchmark packing\n\n");

    uint16_t share[FRODO_N * FRODO_BAR_N] = {0};
    uint8_t packed[FRODO_PACKED_SHARE_LENGTH] = {0};
    uint16_t recovered_share[FRODO_N * FRODO_BAR_N] = {0};

    for(int i = 0; i < FRODO_N * FRODO_BAR_N; i++)
    {
        share[i] = ((uint16_t)RAND_byte(&RNG) << 8) | ((uint16_t)RAND_byte(&RNG));
    }

    // Pack and unpack
    FRODO_pack_share(packed, share);
    FRODO_unpack_share(recovered_share, packed);

    // Packing
    BENCHTEST("packing", FRODO_pack_share(packed, share);, 1000, times);

    // Unpacking
    BENCHTEST("unpacking", FRODO_unpack_share(recovered_share, packed);, 1000, times);

    return 0;
}
