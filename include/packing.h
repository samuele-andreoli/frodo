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

#ifndef FRODO_PACKING_H
#define FRODO_PACKING_H

#include "frodo_config.h"
#include "backend.h"
#include "distributions.h"
#include "lwe_reconciliation.h"
#include "lwe_generate.h"

/* Packing parameters */
#define FRODO_PACKED_SHARE_LENGTH (FRODO_BAR_N * FRODO_N * FRODO_MODULUS + 7) >> 3

/* Packing/Unpacking utilities */

/** @brief Pack the key share for transmission.
 *
 * @param dst, array to store the packed share
 * @param src, key share to pack
 */
void FRODO_pack_share(uint8_t dst[FRODO_PACKED_SHARE_LENGTH], uint16_t* src);

/** @brief Unpack the received key share.
 *
 * @param dst, array to store the unpacked share
 * @param src, the packed key share
 */
void FRODO_unpack_share(uint16_t* dst, uint8_t src[FRODO_PACKED_SHARE_LENGTH]);
#endif
