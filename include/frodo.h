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

#ifndef FRODO_H
#define FRODO_H

#include "amcl.h"

#include "frodo_config.h"
#include "distributions.h"
#include "lwe_generate.h"
#include "lwe_reconciliation.h"
#include "packing.h"

typedef struct {
   uint16_t private[FRODO_BAR_N][FRODO_N];
   uint16_t public[FRODO_BAR_N][FRODO_N];
} FRODO_left_keypair;

typedef struct {
   uint16_t private[FRODO_N][FRODO_BAR_N];
   uint16_t public[FRODO_N][FRODO_BAR_N];
} FRODO_right_keypair;

/* Keypair generation API */

void FRODO_generate_left_keypair(csprng *RNG, FRODO_left_keypair *k, uint8_t seed[FRODO_SEED_LENGTH]);

void FRODO_generate_right_keypair(csprng *RNG, FRODO_right_keypair *k, uint8_t seed[FRODO_SEED_LENGTH]);

void FRODO_generate_left_keypair_full_param(csprng *RNG, FRODO_left_keypair *k, uint16_t a[FRODO_N][FRODO_N]);

void FRODO_generate_right_keypair_full_param(csprng *RNG, FRODO_right_keypair *k, uint16_t a[FRODO_N][FRODO_N]);

/* Protocol run API */

void FRODO_right_keyshare_pack(uint8_t b[FRODO_PACKED_SHARE_LENGTH], FRODO_right_keypair *k);

void FRODO_right_key_agreement(uint8_t key[FRODO_KEY_LENGTH], uint8_t u[FRODO_PACKED_SHARE_LENGTH], FRODO_right_keypair *k, uint8_t hint[FRODO_HINT_LENGTH]);

void FRODO_left_key_agreement(csprng *RNG, uint8_t key[FRODO_KEY_LENGTH], uint8_t u[FRODO_PACKED_SHARE_LENGTH], uint8_t hint[FRODO_HINT_LENGTH], uint8_t b[FRODO_PACKED_SHARE_LENGTH], FRODO_left_keypair *k);

#endif
