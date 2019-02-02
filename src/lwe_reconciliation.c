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

#include "lwe_reconciliation.h"

/* Auxiliary parameters and macros */
// Extract the necessary bits from a rounded matrix element
#define PACK_KEY(b) (uint8_t)((b) >> FRODO_BAR_BITRATE & bitrate_mask)
// Extract a hint bit from position i of the hint h
#define HINT_BIT(h,i) (h[i>>3] >> (i & 0x07) & 1)
// Compute the bias from the hint bit b, given the element v to be rounded
#define HINT_BIAS(v, b) (uint16_t)(((((v) >> (FRODO_BAR_BITRATE - 2)) + 1) >> 1 & 1) * (((b) << 1) - 1) * hint_rounder)

const uint16_t bitrate_mask = (1 << FRODO_BITRATE) - 1;
const uint16_t bitrate_rounder = 1 << (FRODO_BAR_BITRATE - 1);
const uint16_t hint_rounder = 1 << (FRODO_BAR_BITRATE - 2);

/* Key shares composition */
// dst = dst + lr
void FRODO_key_mul(uint16_t dst[FRODO_BAR_N][FRODO_BAR_N], uint16_t l[FRODO_BAR_N][FRODO_N], uint16_t r[FRODO_N][FRODO_BAR_N])
{
   for(int i=0;i<FRODO_BAR_N;i++)
   {
      for(int j=0;j<FRODO_BAR_N;j++)
      {
         for(int k=0;k<FRODO_N;k++)
         {
            dst[i][j]+=l[i][k]*r[k][j];
         }
      }
   }
}

/* Reconciliation functions */
void FRODO_hint(uint8_t hint[FRODO_HINT_LENGTH], uint16_t* V)
{
   for (int i = FRODO_BAR_N * FRODO_BAR_N - 1, j = FRODO_HINT_LENGTH - 1; i > -1; j--)
   {
      for(int k=0; k<8; k++)
      {
         hint[j] <<= 1;
         hint[j] |= (uint8_t)(V[i] >> (FRODO_BAR_BITRATE -1) & 1);
         i--;
      }
   }
}

void FRODO_reconcilitaion(uint8_t key[FRODO_KEY_LENGTH], uint16_t* V)
{
   for (int i = 0, j = 0; i < FRODO_BAR_N * FRODO_BAR_N; j++)
   {
      for(int k = 0; k < 8; k+=FRODO_BITRATE)
      {
         key[j] <<= FRODO_BITRATE;
         key[j] |= PACK_KEY(V[i] + bitrate_rounder);
         i++;
      }
   }
}

void FRODO_reconcilitaion_with_hint(uint8_t key[FRODO_KEY_LENGTH], uint8_t hint[FRODO_HINT_LENGTH], uint16_t* V)
{
   for (int i = 0, j = 0; i < FRODO_BAR_N * FRODO_BAR_N ; j++)
   {
      for(int k = 0; k < 8; k+=FRODO_BITRATE)
      {
         key[j] <<= FRODO_BITRATE;
         key[j] |= PACK_KEY((V[i] + bitrate_rounder + HINT_BIAS(V[i], HINT_BIT(hint, i))));
         i++;
      }
   }
}

#ifdef HAS_MAIN
#include <stdlib.h>
#include <stdio.h>

int main()
{
   // Use same matrix for testing
   uint16_t m[FRODO_BAR_N][FRODO_BAR_N];
   for(int i = 0; i < FRODO_BAR_N; i++)
   {
      for(int j = 0; j < FRODO_BAR_N; j++)
      {
         m[i][j] = rand();
      }
   }

   // Test hint computation
   uint8_t hint[FRODO_HINT_LENGTH] = {0};
   FRODO_hint(hint, (uint16_t*)m);
   for(int i = 0; i< FRODO_HINT_LENGTH; i++)
   {
      printf("%02x:", hint[i]);
   }
   printf("\n");

   // Test reconciliation
   uint8_t key1[FRODO_KEY_LENGTH] = {0};
   FRODO_reconcilitaion(key1, (uint16_t*)m);

   for(int i=0; i<FRODO_KEY_LENGTH; i++)
   {
      printf("%02x:", key1[i]);
   }
   printf("\n");

   // Test reconciliation with hint
   uint8_t key2[FRODO_KEY_LENGTH] = {0};
   FRODO_reconcilitaion_with_hint(key2, hint, (uint16_t*)m);
   for(int i=0; i<FRODO_KEY_LENGTH; i++)
   {
      printf("%02x:", key2[i]);
   }
   printf("\n");

   // Diff mask
   for(int i=0; i<FRODO_KEY_LENGTH; i++)
   {
      printf("%02x:", key1[i]^key2[i]);
   }
   printf("\n");

}
#endif