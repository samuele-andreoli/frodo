/*
   Copyright 2018 Samuele Andreoli

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
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
