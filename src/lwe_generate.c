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

const int a_len = FRODO_N * FRODO_N * sizeof(uint16_t);
const int stripe_len = FRODO_A_STRIPE_LEN * sizeof(uint16_t);

/* Parameter generation functions */
void FRODO_generate_a(uint16_t a[FRODO_N][FRODO_N], uint8_t seed[FRODO_SEED_LENGTH])
{
    FRODO_AES_CTX aes_ctx;
    uint16_t i, j;

    for(i=0; i<FRODO_N; i++)
    {
        for(j=0; j<FRODO_N; j+=FRODO_A_STRIPE_LEN)
        {
            a[i][j] = i;
            a[i][j+1] = j;
        }
    }

    FRODO_AES_ECB_INIT(&aes_ctx, seed);
    FRODO_AES_ECB_ENCRYPT_BUFFER(&aes_ctx, a, a_len);

    // No need to clean AES, as the seed for the parameter a is public
}

// dst = dst + sa
void FRODO_parameter_left_mul(uint16_t dst[FRODO_BAR_N][FRODO_N], uint16_t s[FRODO_BAR_N][FRODO_N], uint16_t a[FRODO_N][FRODO_N])
{
    uint16_t i, j, k, col[FRODO_BAR_N];

    for(k=0; k<FRODO_N; k++)
    {
        for(i=0; i<FRODO_BAR_N; i++)
        {
            col[i] = s[i][k];
        }

        for(i=0; i<FRODO_BAR_N; i++)
        {
            for(j=0; j<FRODO_N; j++)
            {
                dst[i][j]+=col[i]*a[k][j];
            }
        }
    }
}

// dst = dst + as
void FRODO_parameter_right_mul(uint16_t dst[FRODO_N][FRODO_BAR_N], uint16_t s[FRODO_N][FRODO_BAR_N], uint16_t a[FRODO_N][FRODO_N])
{
    uint16_t i, j, k;

    for(i=0; i<FRODO_N; i++)
    {
        for(k=0; k<FRODO_N; k++)
        {
            for(j=0; j<FRODO_BAR_N; j++)
            {
                dst[i][j]+=a[i][k]*s[k][j];
            }
        }
    }
}

void FRODO_generate_multiply_by_row(uint16_t dst[FRODO_N][FRODO_BAR_N], uint16_t s[FRODO_N][FRODO_BAR_N], uint8_t seed[FRODO_SEED_LENGTH])
{
    FRODO_AES_CTX aes_ctx;
    uint16_t i, j, k;
#if FRODO_OPTIMIZATION==FRODO_OPTIMIZE_MEMORY
    uint16_t l;
#endif

    FRODO_AES_ECB_INIT(&aes_ctx, seed);

    for(i=0; i<FRODO_N; i++)
    {
#if FRODO_OPTIMIZATION==FRODO_OPTIMIZE_MEMORY
        for(j=0; j<FRODO_N; j+=FRODO_A_STRIPE_LEN)
        {
            INIT_STRIPE(stripe, i, j);
            FRODO_AES_ECB_ENCRYPT_SINGLE(&aes_ctx, stripe);

            for(k=0; k<FRODO_A_STRIPE_LEN; k++)
            {
                for(l=0; l<FRODO_BAR_N; l++)
                {
                    dst[i][l]+=(uint16_t)(stripe[k]*s[j+k][l]);
                }
            }
        }
#elif FRODO_OPTIMIZATION==FRODO_OPTIMIZE_TIME
        uint16_t row[FRODO_N] = {0};
        for(j=0; j<FRODO_N; j+=FRODO_A_STRIPE_LEN)
        {
            row[j] = i;
            row[j+1] = j;
        }

        FRODO_AES_ECB_ENCRYPT_BUFFER(&aes_ctx, row, 2 * FRODO_N);

        for(k=0; k<FRODO_N; k++)
        {
            for(j=0; j<FRODO_BAR_N; j++)
            {
                dst[i][j]+=(uint16_t)(row[k]*s[k][j]);
            }
        }
#endif
    }

    // No need to clean AES, as the seed for the parameter a is public
}

void FRODO_generate_multiply_by_column(uint16_t dst[FRODO_BAR_N][FRODO_N], uint16_t s[FRODO_BAR_N][FRODO_N], uint8_t seed[FRODO_SEED_LENGTH])
{
    FRODO_AES_CTX aes_ctx;
    uint16_t i, j, k;
#if FRODO_OPTIMIZATION==FRODO_OPTIMIZE_MEMORY
    uint16_t l;
#elif FRODO_OPTIMIZATION==FRODO_OPTIMIZE_TIME
    uint16_t col[FRODO_BAR_N];
#endif

    FRODO_AES_ECB_INIT(&aes_ctx, seed);

    for(i=0; i<FRODO_N; i++)
    {
#if FRODO_OPTIMIZATION==FRODO_OPTIMIZE_MEMORY
        for(j=0; j<FRODO_N; j+=FRODO_A_STRIPE_LEN)
        {
            INIT_STRIPE(stripe, i, j);
            FRODO_AES_ECB_ENCRYPT_SINGLE(&aes_ctx, stripe);

            for(l=0; l<FRODO_BAR_N; l++)
            {
                for(k=0; k<FRODO_A_STRIPE_LEN; k++)
                {
                    dst[l][j + k]+=(uint16_t)(stripe[k]*s[l][i]);
                }
            }
        }
#elif FRODO_OPTIMIZATION==FRODO_OPTIMIZE_TIME
        uint16_t row[FRODO_N] = {0};
        for(j=0; j<FRODO_N; j+=FRODO_A_STRIPE_LEN)
        {
            row[j] = i;
            row[j+1] = j;
        }

        FRODO_AES_ECB_ENCRYPT_BUFFER(&aes_ctx, row, 2 * FRODO_N);

        for(k=0; k<FRODO_BAR_N; k++)
        {
            col[k] = s[k][i];
        }

        for(k=0; k<FRODO_BAR_N; k++)
        {
            for(j=0; j<FRODO_N; j++)
            {
                dst[k][j]+=(uint16_t)(row[j]*col[k]);
            }
        }
#endif
    }

    // No need to clean AES, as the seed for the parameter a is public
}

#ifdef HAS_MAIN
int main()
{
    FRODO_CSPRNG RNG;
    char rng_seed[100];
    for (int i=0; i<100; i++)
        rng_seed[i] = i;
    RAND_seed(&RNG, 100, rng_seed);

    char seed[FRODO_SEED_LENGTH];
    for(int i=0; i<FRODO_SEED_LENGTH; i++)
    {
        seed[i] = (char)RAND_byte(&RNG);
    }

    uint16_t sl[FRODO_BAR_N][FRODO_N];
    uint16_t sr[FRODO_N][FRODO_BAR_N];
    for(int i=0; i<FRODO_BAR_N; i++)
    {
        for(int j=0; j<FRODO_N; j++)
        {
            sl[i][j]=j+FRODO_N*i;
        }
    }
    for(int i=0; i<FRODO_N; i++)
    {
        for(int j=0; j<FRODO_BAR_N; j++)
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
    for(int i =0; i<FRODO_N; i++)
    {
        for(int j=0; j<FRODO_N; j++)
        {
            fprintf(data, "%04X ", a[i][j]);
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
    for(int i=0; i<FRODO_BAR_N; i++)
    {
        for(int j=0; j<FRODO_N; j++)
        {
            fprintf(data, "%04X ", sa_auto[i][j]);
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
    for(int i=0; i<FRODO_BAR_N; i++)
    {
        for(int j=0; j<FRODO_N; j++)
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
    for(int i=0; i<FRODO_N; i++)
    {
        for(int j=0; j<FRODO_BAR_N; j++)
        {
            fprintf(data, "%04X ", as_auto[i][j]);
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
    for(int i=0; i<FRODO_N; i++)
    {
        for(int j=0; j<FRODO_BAR_N; j++)
        {
            fprintf(data, "%d ", as_auto[i][j]==as_man[i][j]);
        }
        fprintf(data, "\n");
    }
    fclose(data);

    return 0;
}
#endif