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

#include "lwe_generate.h"

/* Auxiliary macros */
#define INIT_STRIPE(stripe, row, column)       \
    uint16_t stripe[FRODO_A_STRIPE_LEN] = {0}; \
    stripe[0] = row;                           \
    stripe[1] = column;

/* Parameter generation functions */
void FRODO_generate_a(uint16_t a[FRODO_N][FRODO_N], uint8_t seed[FRODO_SEED_LENGTH])
{
    amcl_aes aes;
    uint16_t i, j;

    // Willingly ignore the exit code
    // It only fails with exit code if the key has an invalid length,
    // but FRODO_SEED_LENGTH is checked to be valid at compile time in the header file
    AES_init(&aes, ECB, FRODO_SEED_LENGTH, (char*)seed, NULL);

    for(i=0;i<FRODO_N;i++)
    {
        for(j=0;j<FRODO_N;j+=FRODO_A_STRIPE_LEN)
        {
            a[i][j]=i;
            a[i][j+1]=j;
            AES_ecb_encrypt(&aes, (uchar*)&a[i][j]);
        }
    }
}

// dst = dst + sa
void FRODO_parameter_left_mul(uint16_t dst[FRODO_BAR_N][FRODO_N], uint16_t s[FRODO_BAR_N][FRODO_N], uint16_t a[FRODO_N][FRODO_N])
{
    for(int i=0;i<FRODO_BAR_N;i++)
    {
        for(int j=0;j<FRODO_N;j++)
        {
            for(int k=0;k<FRODO_N;k++)
            {
                dst[i][j]+=s[i][k]*a[k][j];
            }
        }
    }
}

// dst = dst + as
void FRODO_parameter_right_mul(uint16_t dst[FRODO_N][FRODO_BAR_N], uint16_t s[FRODO_N][FRODO_BAR_N], uint16_t a[FRODO_N][FRODO_N])
{
    for(int i=0;i<FRODO_N;i++)
    {
        for(int j=0;j<FRODO_BAR_N;j++)
        {
            for(int k=0;k<FRODO_N;k++)
            {
                dst[i][j]+=a[i][k]*s[k][j];
            }
        }
    }
}

void FRODO_generate_multiply_by_row(uint16_t dst[FRODO_N][FRODO_BAR_N], uint16_t s[FRODO_N][FRODO_BAR_N], uint8_t seed[FRODO_SEED_LENGTH])
{
    amcl_aes aes;
    uint16_t i, j, k, l;

    // Willingly ignore the exit code
    // It only fails with exit code if the key has an invalid length,
    // but FRODO_SEED_LENGTH is checked to be valid at compile time in the header file
    AES_init(&aes, ECB, FRODO_SEED_LENGTH, (char*)seed, NULL);

    for(i=0;i<FRODO_N;i++)
    {
        for(j=0;j<FRODO_N;j+=FRODO_A_STRIPE_LEN)
        {
            INIT_STRIPE(stripe, i, j);
            AES_ecb_encrypt(&aes, (uchar*)stripe);

            for(l=0;l<FRODO_BAR_N;l++)
            { 
                for(k=0;k<FRODO_A_STRIPE_LEN;k++)
                {
                    dst[i][l]+=(uint16_t)(stripe[k]*s[j+k][l]);
                }
            }
        }
    }
}

void FRODO_generate_multiply_by_column(uint16_t dst[FRODO_BAR_N][FRODO_N], uint16_t s[FRODO_BAR_N][FRODO_N], uint8_t seed[FRODO_SEED_LENGTH])
{
    amcl_aes aes;
    uint16_t i, j, k, l;

    // Willingly ignore the exit code
    // It only fails with exit code if the key has an invalid length,
    // but FRODO_SEED_LENGTH is checked to be valid at compile time in the header file
    AES_init(&aes, ECB, FRODO_SEED_LENGTH, (char*)seed, NULL);

    for(j=0;j<FRODO_N;j+=FRODO_A_STRIPE_LEN)
    {
        for(i=0;i<FRODO_N;i++)
        {
            INIT_STRIPE(stripe, i, j);
            AES_ecb_encrypt(&aes, (uchar*)stripe);

            for(k=0;k<FRODO_A_STRIPE_LEN;k++)
            { 
                for(l=0;l<FRODO_BAR_N;l++)
                {
                    dst[l][j+k]+=(uint16_t)(stripe[k]*s[l][i]);
                }
            }
        }
    }
}

#ifdef HAS_MAIN
int main()
{
    csprng RNG;
    char rng_seed[100];
    for (int i=0;i<100;i++)
        rng_seed[i] = i;
    RAND_seed(&RNG, 100, rng_seed);

    char seed[FRODO_SEED_LENGTH];
    for(int i=0;i<FRODO_SEED_LENGTH;i++)
    {
        seed[i] = (char)RAND_byte(&RNG);
    }

    uint16_t sl[FRODO_BAR_N][FRODO_N];
    uint16_t sr[FRODO_N][FRODO_BAR_N];
    for(int i=0;i<FRODO_BAR_N;i++)
    {
        for(int j=0;j<FRODO_N;j++)
        {
            sl[i][j]=j+FRODO_N*i;
        }
    }
    for(int i=0;i<FRODO_N;i++)
    {
        for(int j=0;j<FRODO_BAR_N;j++)
        {
            sr[i][j]=j+FRODO_BAR_N*i;
        }
    }
    uint16_t a[FRODO_N][FRODO_N] = {0};
    uint16_t as_auto[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t as_man[FRODO_N][FRODO_BAR_N] = {0};
    uint16_t sa_auto[FRODO_BAR_N][FRODO_N] = {0};
    uint16_t sa_man[FRODO_BAR_N][FRODO_N] = {0};

    FRODO_generate_a(a, seed);
    FRODO_generate_multiply_by_column(sa_auto, sl, seed);
    FRODO_generate_multiply_by_row(as_auto, sr, seed);
    FRODO_parameter_left_mul(sa_man, sl, a);
    FRODO_parameter_right_mul(as_man, sr, a);

    FILE* data = fopen("matrix.dat", "w");
    if(data == NULL)
    {
        printf("Error opening matrix datafile");
        return 1;
    }
    for(int i =0;i<FRODO_N;i++)
    {
        for(int j=0;j<FRODO_N;j++)
        {
            fprintf(data, "%x ", a[i][j]);
        }
        fprintf(data, "\n");
    }
    fclose(data);

    data = fopen("sa.dat", "w");
    if(data == NULL)
    {
        printf("Error opening sa datafile");
        return 1;
    }
    for(int i=0;i<FRODO_BAR_N;i++) 
    {
        for(int j=0;j<FRODO_N;j++)
        {
            fprintf(data, "%d ", sa_auto[i][j]);
        }
        fprintf(data, "\n");
    }
    fclose(data);

    data = fopen("sa_bool.dat", "w");
    if(data == NULL)
    {
        printf("Error opening sa_bool datafile");
        return 1;
    }
    for(int i=0;i<FRODO_BAR_N;i++)
    {
        for(int j=0;j<FRODO_N;j++)
        {
            fprintf(data, "%d ", sa_auto[i][j]==sa_man[i][j]);
        }
        fprintf(data, "\n");
    }
    fclose(data);

    data = fopen("as.dat", "w");
    if(data == NULL)
    {
        printf("Error opening as datafile");
        return 1;
    }
    for(int i=0;i<FRODO_N;i++)
    {
        for(int j=0;j<FRODO_BAR_N;j++)
        {
            fprintf(data, "%d ", as_auto[i][j]);
        }
        fprintf(data, "\n");
    }

    fclose(data);
    data = fopen("as_bool.dat", "w");
    if(data == NULL)
    {
        printf("Error opening as_bool datafile");
        return 1;
    }
    for(int i=0;i<FRODO_N;i++)
    {
        for(int j=0;j<FRODO_BAR_N;j++)
        {
            fprintf(data, "%d ", as_auto[i][j]==as_man[i][j]);
        }
        fprintf(data, "\n");
    }
    fclose(data);

    return 0;
}
#endif