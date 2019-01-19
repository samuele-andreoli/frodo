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

#ifndef FRODO_CONFIG_H
#define FRODO_CONFIG_H

#define CT_ASSERT(exp,msg) enum {ct_assertion__##msg = 1/(exp)}

/* Frodo configurations */
#define FRODO_TOY 1
#define FRODO_CLASSIC 2
#define FRODO_RECOMMENDED 3
#define FRODO_PARANOID 4

// Default configuration
#ifndef FRODO_CONFIGURATION
#define FRODO_CONFIGURATION FRODO_RECOMMENDED
#endif

/* Error distribution */
#define FRODO_D1 1
#define FRODO_D2 2
#define FRODO_D3 3
#define FRODO_D4 4

#if FRODO_CONFIGURATION == FRODO_TOY
#define FRODO_DISTRIBUTION FRODO_D1
#elif FRODO_CONFIGURATION == FRODO_CLASSIC
#define FRODO_DISTRIBUTION FRODO_D2
#elif FRODO_CONFIGURATION == FRODO_RECOMMENDED
#define FRODO_DISTRIBUTION FRODO_D3
#elif FRODO_CONFIGURATION == FRODO_PARANOID
#define FRODO_DISTRIBUTION FRODO_D4
#endif

/* Parameter generation */
#define FRODO_A_STRIPE_LEN 8
#define FRODO_SEED_LENGTH 16

/* LWE dimensions */
#if FRODO_CONFIGURATION == FRODO_TOY
#define FRODO_N 352
#define FRODO_BAR_N 8
#elif FRODO_CONFIGURATION == FRODO_CLASSIC
#define FRODO_N 592
#define FRODO_BAR_N 8
#elif FRODO_CONFIGURATION == FRODO_RECOMMENDED
#define FRODO_N 752
#define FRODO_BAR_N 8
#elif FRODO_CONFIGURATION == FRODO_PARANOID
#define FRODO_N 864
#define FRODO_BAR_N 8
#endif

/* Reconciliation parameters */
#if FRODO_CONFIGURATION == FRODO_TOY
#define FRODO_BITRATE 1
#define FRODO_MODULUS 11
#elif FRODO_CONFIGURATION == FRODO_CLASSIC
#define FRODO_BITRATE 2
#define FRODO_MODULUS 12
#elif FRODO_CONFIGURATION == FRODO_RECOMMENDED
#define FRODO_BITRATE 4
#define FRODO_MODULUS 15
#elif FRODO_CONFIGURATION == FRODO_PARANOID
#define FRODO_BITRATE 4
#define FRODO_MODULUS 15
#endif

#define FRODO_BAR_BITRATE (FRODO_MODULUS - FRODO_BITRATE)
#define FRODO_HINT_LENGTH (FRODO_BAR_N * FRODO_BAR_N >> 3)
#define FRODO_KEY_LENGTH (FRODO_BITRATE * FRODO_HINT_LENGTH)

/* Compile time checks on configuration */

// Checks on LWE dimensions
CT_ASSERT(!(FRODO_N % FRODO_A_STRIPE_LEN), stripe_len_must_divide_A_len);
CT_ASSERT(FRODO_SEED_LENGTH == 16 | FRODO_SEED_LENGTH == 24 | FRODO_SEED_LENGTH == 32, seed_must_be_valid_AES_key);

// Checks on reconciliation
CT_ASSERT(!(FRODO_BITRATE>FRODO_MODULUS), bitrate_must_not_be_grater_than_modulus);
CT_ASSERT(FRODO_BAR_N * FRODO_BAR_N % 8 == 0, hint_must_be_integer_number_of_bytes);
CT_ASSERT(8 % FRODO_BITRATE == 0, birate_must_divide_8);
#endif
