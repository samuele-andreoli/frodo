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

#include "distributions.h"

#define N_SAMPLES 100000

#if FRODO_DISTRIBUTION == FRODO_D1
#define CHI_TH 24.332
#define FRODO_CDF_LENGTH 4
const uint16_t CDF[FRODO_CDF_LENGTH] = {43, 104, 124, 127};
#elif FRODO_DISTRIBUTION == FRODO_D2
#define CHI_TH 27.877
#define FRODO_CDF_LENGTH 5
const uint16_t CDF[FRODO_CDF_LENGTH] = {784, 1774, 2022, 2046, 2047};
#elif FRODO_DISTRIBUTION == FRODO_D3
#define CHI_TH 34.528
#define FRODO_CDF_LENGTH 6
const uint16_t CDF[FRODO_CDF_LENGTH] = {602, 1521, 1927, 2031, 2046, 2047};
#elif FRODO_DISTRIBUTION == FRODO_D4
#define CHI_TH 37.697
#define FRODO_CDF_LENGTH 7
const uint16_t CDF[FRODO_CDF_LENGTH] = {9651, 24351, 30841, 32500, 32745, 32766, 32767};
#endif

/* Perform Chi Square test with p=.999 */

int main() {
    csprng RNG;
    char seed[100];

    for (int i=0; i<100; i++)
        seed[i] = i;
    RAND_seed(&RNG, 100, seed);

    uint16_t samples[N_SAMPLES] = {0};
    int counts[2*FRODO_CDF_LENGTH-1] = {0};
    int zero_pos = FRODO_CDF_LENGTH-1;

    FRODO_inverse_sample(&RNG, samples, N_SAMPLES);

    for(int i=0; i<N_SAMPLES; i++){
        if ((samples[i] < FRODO_CDF_LENGTH) && (samples[i] > FRODO_CDF_LENGTH))
        {
            printf("Generated sample out of bounds: %d\n", samples[i]);
            return 1;
        }
        counts[zero_pos+(int16_t)samples[i]]++;
    }

    // Chi squared test
    double weight = N_SAMPLES / (2 * (CDF[FRODO_CDF_LENGTH-1] + 1));

    double expectations[2*FRODO_CDF_LENGTH-1];
    expectations[zero_pos] = CDF[0] * weight;
    for(int i=1; i<FRODO_CDF_LENGTH; i++)
    {
        expectations[zero_pos+i] = (CDF[i] - CDF[i-1]) * weight;
        expectations[zero_pos-i] = (CDF[i] - CDF[i-1]) * weight; 
    }

    double chi_squared = 0;
    for(int i=0; i<2*FRODO_CDF_LENGTH-1; i++)
    {
        chi_squared += (counts[i] - expectations[i]) / expectations[i];
    }

    if(CHI_TH < chi_squared)
    {
        printf("Chi square test failed.\nThreshold for %d degrees of freedom: %lf.\nChi square statistic: %lf\n", 2 * FRODO_CDF_LENGTH - 1, CHI_TH, chi_squared);
        return 1;
    }

    printf("Chi square: %lf\n", chi_squared);

    return 0;
}