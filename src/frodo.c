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

#include "frodo.h"

#define FRODO_MEMORY_CLEAR(v, s) for(int i = 0; i < s; i++) ((uint16_t*)v)[i] = 0;

void FRODO_generate_left_keypair(FRODO_CSPRNG *RNG, FRODO_left_keypair *k, uint8_t seed[FRODO_SEED_LENGTH])
{
    // Sample secret key
    FRODO_inverse_sample(RNG, (uint16_t*)k->private, FRODO_BAR_N * FRODO_N);

    // Sample error
    FRODO_inverse_sample(RNG, (uint16_t*)k->public, FRODO_BAR_N * FRODO_N);
    
    // Compute public key
    FRODO_generate_multiply_by_column(k->public, k->private, seed);
}

void FRODO_generate_right_keypair(FRODO_CSPRNG *RNG, FRODO_right_keypair *k, uint8_t seed[FRODO_SEED_LENGTH])
{
    // Sample secret key
    FRODO_inverse_sample(RNG, (uint16_t*)k->private, FRODO_N * FRODO_BAR_N);

    // Sample error
    FRODO_inverse_sample(RNG, (uint16_t*)k->public, FRODO_N * FRODO_BAR_N);
    
    // Compute public key
    FRODO_generate_multiply_by_row(k->public, k->private, seed);
}

void FRODO_generate_left_keypair_full_param(FRODO_CSPRNG *RNG, FRODO_left_keypair *k, uint16_t a[FRODO_N][FRODO_N])
{
    // Sample secret key
    FRODO_inverse_sample(RNG, (uint16_t*)k->private, FRODO_BAR_N * FRODO_N);

    // Sample error
    FRODO_inverse_sample(RNG, (uint16_t*)k->public, FRODO_BAR_N * FRODO_N);
    
    // Compute public key
    FRODO_parameter_left_mul(k->public, k->private, a);
}

void FRODO_generate_right_keypair_full_param(FRODO_CSPRNG *RNG, FRODO_right_keypair *k, uint16_t a[FRODO_N][FRODO_N])
{
    // Sample secret key
    FRODO_inverse_sample(RNG, (uint16_t*)k->private, FRODO_BAR_N * FRODO_N);

    // Sample error
    FRODO_inverse_sample(RNG, (uint16_t*)k->public, FRODO_BAR_N * FRODO_N);

    // Compute public key
    FRODO_parameter_right_mul(k->public, k->private, a);
}

void FRODO_right_keyshare_pack(uint8_t b[FRODO_PACKED_SHARE_LENGTH], FRODO_right_keypair *k)
{
    FRODO_pack_share(b, (uint16_t*)k->public);
}

void FRODO_right_key_agreement(uint8_t key[FRODO_KEY_LENGTH], uint8_t u[FRODO_PACKED_SHARE_LENGTH], FRODO_right_keypair *k, uint8_t hint[FRODO_HINT_LENGTH])
{
    uint16_t left_keyshare[FRODO_BAR_N][FRODO_N] = {0};
    uint16_t combined_shares[FRODO_BAR_N][FRODO_BAR_N] = {0};

    // Unpack left keyshare
    FRODO_unpack_share((uint16_t*)left_keyshare, u);

    // Compute key
    FRODO_key_mul(combined_shares, left_keyshare, k->private);
    FRODO_reconcilitaion_with_hint(key, hint, (uint16_t*)combined_shares);

    // Zero out combined shares
    FRODO_MEMORY_CLEAR(combined_shares, FRODO_BAR_N * FRODO_BAR_N);
}

void FRODO_left_key_agreement(FRODO_CSPRNG *RNG, uint8_t key[FRODO_KEY_LENGTH], uint8_t u[FRODO_PACKED_SHARE_LENGTH], uint8_t hint[FRODO_HINT_LENGTH], uint8_t b[FRODO_PACKED_SHARE_LENGTH], FRODO_left_keypair *k)
{
    uint16_t right_keyshare[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t combined_shares[FRODO_BAR_N][FRODO_BAR_N] = {0};

    // Unpack right keyshare
    FRODO_unpack_share((uint16_t*)right_keyshare, b);

    // Compute key
    FRODO_inverse_sample(RNG, (uint16_t*)combined_shares, FRODO_BAR_N * FRODO_BAR_N);
    FRODO_key_mul(combined_shares, k->private, right_keyshare);
    FRODO_reconcilitaion(key, (uint16_t*)combined_shares);

    // Pack keyshare
    FRODO_pack_share(u, (uint16_t*)k->public);

    // Compute hint
    FRODO_hint(hint, (uint16_t*)combined_shares);

    // Zero out combined shares
    FRODO_MEMORY_CLEAR(combined_shares, FRODO_BAR_N * FRODO_BAR_N);
}
