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

#ifndef LWE_GENERATE_H
#define LWE_GENERATE_H

#include "amcl.h"

#include "frodo_config.h"

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
