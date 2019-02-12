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

#ifndef LWE_GENERATE_H
#define LWE_GENERATE_H

#include "frodo_config.h"
#include "backend.h"

/* Matrix generation API */

/** @brief Generate the full parameter a from a seed and store it [useful for caching]
 *
 * @param a, a matrix to store the generated parameter. Assumed to be already zeroed.
 * @param seed, the seed to generate the parameter
 */
void FRODO_generate_a(uint16_t a[FRODO_N][FRODO_N], uint8_t seed[FRODO_SEED_LENGTH]);

/** @brief Multiply parameter a and matrix s and add the result to dst: dst += sa
 *
 * @param dst, a matrix to store the product. The result of as is added to dst
 * @param s, the matrix to be multiplied with the parameter
 * @param a, generated parameter.
 */
void FRODO_parameter_left_mul(uint16_t dst[FRODO_BAR_N][FRODO_N], uint16_t s[FRODO_BAR_N][FRODO_N], uint16_t a[FRODO_N][FRODO_N]);

/** @brief Multiply parameter a and matrix s and add the result to dst: dst += as
 *
 * @param dst, a matrix to store the product. The result of as is added to dst
 * @param s, the matrix to be multiplied with the parameter
 * @param a, generated parameter.
 */
void FRODO_parameter_right_mul(uint16_t dst[FRODO_N][FRODO_BAR_N], uint16_t s[FRODO_N][FRODO_BAR_N], uint16_t a[FRODO_N][FRODO_N]);

/** @brief Generate the parameter by row and multiply it with a provided matrix. The final result is dst + a*s
 *
 * @param dst, a matrix to store the result
 * @param s, the matrix to be multplied with the parameter
 * @param seed, the seed to generate the parameter
 */
void FRODO_generate_multiply_by_row(uint16_t dst[FRODO_N][FRODO_BAR_N], uint16_t s[FRODO_N][FRODO_BAR_N], uint8_t seed[FRODO_SEED_LENGTH]);

/** @brief Generate the parameter by columns and multiply it with a provided matrix. The final result is dst + s*a
 *
 * @param dst, a matrix to store the result
 * @param s, the matrix to be multplied with the parameter
 * @param seed, the seed to generate the parameter
 */
void FRODO_generate_multiply_by_column(uint16_t dst[FRODO_BAR_N][FRODO_N], uint16_t s[FRODO_BAR_N][FRODO_N], uint8_t seed[FRODO_SEED_LENGTH]);

#endif
