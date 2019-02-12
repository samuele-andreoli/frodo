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

#ifndef LWE_RECONCILIATION_H
#define LWE_RECONCILIATION_H

#include "frodo_config.h"
#include "backend.h"

/* Reconciliation API */

/** @brief Multiply the partial key shares and add the result to dst: dst += lr
 *
 * @param dst, matrix to store the product. The result of lr is added to dst
 * @param l, left key share.
 * @param r, right key share
 */
void FRODO_key_mul(uint16_t dst[FRODO_BAR_N][FRODO_BAR_N], uint16_t l[FRODO_BAR_N][FRODO_N], uint16_t r[FRODO_N][FRODO_BAR_N]);

/** @brief Compute the hint for key reconciliation
 *
 * @param hint, buffer to hold the hint. It MUST be zeroed
 * @param V, combined key shares
 */
void FRODO_hint(uint8_t hint[FRODO_HINT_LENGTH], uint16_t* V);

/** @brief Compute the key from the combined shares
 *
 * @param key, buffer to hold the key. It MUST be zeroed
 * @param V, combined key shares
 */
void FRODO_reconcilitaion(uint8_t key[FRODO_KEY_LENGTH], uint16_t* V);

/** @brief Compute the key from the combined shares using an hint
 *
 * @param key, buffer to hold the key. It MUST be zeroed
 * @param hint, hint for key reconciliation
 * @param V, combined key shares
 */
void FRODO_reconcilitaion_with_hint(uint8_t key[FRODO_KEY_LENGTH], uint8_t hint[FRODO_HINT_LENGTH], uint16_t* V);
#endif
