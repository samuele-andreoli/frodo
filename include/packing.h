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

#ifndef FRODO_PACKING_H
#define FRODO_PACKING_H

#include <stdint.h>

#include "frodo_config.h"
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
