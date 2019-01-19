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

#include "packing.h"

const uint8_t masks[8] = {0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01};

/* Packing/Unpacking utilities */
void FRODO_pack_share(uint8_t dst[FRODO_PACKED_SHARE_LENGTH], uint16_t* src)
{
#if FRODO_MODULUS == 12
   for(int i = 0, j = 0; j < FRODO_PACKED_SHARE_LENGTH;)
   {
      dst[j++] = src[i] >> 4 & 0xFF;
      dst[j++] = (src[i++] & 0x0F) << 4 | src[i] >> 8 & 0x0F;
      dst[j++] = src[i++] & 0xFF;
   }
#else
   int bit_position = 0;
   int mask_position = 0;

   for(int i = 0, j = 0; j < FRODO_PACKED_SHARE_LENGTH; j++)
   {
      if(bit_position < FRODO_MODULUS - 8)
      {
         dst[j] = src[i] >> (FRODO_MODULUS - 8 - bit_position) & 0xFF;
         bit_position = (bit_position + 8);
      }
      else if(bit_position == FRODO_MODULUS - 8)
      {
         dst[j] = (uint8_t)src[i++];
         bit_position = 0;     
      }
      else
      {
         mask_position = bit_position - FRODO_MODULUS + 8;
         dst[j] = ((src[i] & masks[mask_position]) << mask_position) | (src[i+1] >> (FRODO_MODULUS - mask_position) & masks[8 - mask_position]);
         bit_position = (bit_position + 8) % FRODO_MODULUS;
         i++;
      }
   }
#endif
}

void FRODO_unpack_share(uint16_t* dst, uint8_t src[FRODO_PACKED_SHARE_LENGTH])
{
#if FRODO_MODULUS == 12
   for(int i = 0, j = 0; j < FRODO_PACKED_SHARE_LENGTH;)
   {
      dst[i++] = ((uint16_t)src[j++]) << 4 | src[j] >> 4;
      dst[i++] = ((uint16_t)src[j++]) << 8 | src[j++];
   }
#else
   int bit_position = 0;
   int mask_position  = 0;

   for(int i = 0, j = 0; i < FRODO_BAR_N * FRODO_N;)
   {
      if(bit_position < FRODO_MODULUS - 8)
      {
         dst[i] |= ((uint16_t)src[j++]) << (FRODO_MODULUS - 8 - bit_position);
         bit_position += 8;
      }
      else if(bit_position == FRODO_MODULUS - 8)
      {
         dst[i++] |= (uint16_t)src[j++];
         bit_position = 0;
      }
      else
      {
         mask_position = bit_position - FRODO_MODULUS + 8;
         dst[i++] |= src[j] >> mask_position & masks[mask_position];
         dst[i] |= ((uint16_t)src[j++]) << (FRODO_MODULUS - mask_position);
         bit_position = (bit_position + 8) % FRODO_MODULUS;
      }
   }
#endif
}

#ifdef HAS_MAIN
const uint16_t mask = (1 << FRODO_MODULUS) -1; 

int main()
{
   uint16_t share[FRODO_N * FRODO_BAR_N] = {0};
   for(int i = 0; i < FRODO_N * FRODO_BAR_N; i++)
   {
      share[i] = 0x1234;
   }

   uint8_t packed[FRODO_PACKED_SHARE_LENGTH] = {0};
   FRODO_pack_share(packed, share);
   printf("Test packing share\n");
   for(int i = 0; i < FRODO_PACKED_SHARE_LENGTH; i++)
   {
      printf("%02x:", packed[i]);
   }
   printf("\n");

   uint16_t recovered_share[FRODO_N * FRODO_BAR_N] = {0};
   FRODO_unpack_share(recovered_share, packed);
   for(int i = 0; i < FRODO_N * FRODO_BAR_N; i++)
   {
      printf("%04X:", (share[i] ^ recovered_share[i]) & mask);
   }
   printf("\n");
}
#endif