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

#ifndef LWE_RECONCILIATION_H
#define LWE_RECONCILIATION_H

#include <stdint.h>

#include "frodo_config.h"

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
