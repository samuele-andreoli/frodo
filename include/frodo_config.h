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

/* Optimization */
#define FRODO_OPTIMIZE_MEMORY 1
#define FRODO_OPTIMIZE_TIME 2

#ifndef FRODO_OPTIMIZATION
#define FRODO_OPTIMIZATION FRODO_OPTIMIZE_MEMORY
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
