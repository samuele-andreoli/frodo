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

#ifndef FRODO_BACKEND_H
#define FRODO_BACKEND_H

#define FRODO_BACKEND_MILAGRO 1
#define FRODO_BACKEND_OPENSSL 2

#ifndef FRODO_BACKEND
#define FRODO_BACKEND FRODO_BACKEND_MILAGRO
#endif

/* AES configuration */
#define FRODO_AES_BLOCK_SIZE 16

#if FRODO_BACKEND==FRODO_BACKEND_MILAGRO

#include "amcl.h"

/* Entropy generation macros */
#define FRODO_CSPRNG csprng

#define FRODO_CSPRNG_SEED(rng_ptr, seed, seed_len) RAND_seed(rng_ptr, seed_len, (char*)seed);

#define FRODO_CSPRNG_GET(rng_ptr) RAND_byte(rng_ptr);

/* AES related macros */
#define FRODO_AES_CTX amcl_aes

#define FRODO_AES_ECB_INIT(aes_ctx_ptr, key)\
    AES_init(aes_ctx_ptr, ECB, FRODO_AES_BLOCK_SIZE, (char*)key, NULL);

#define FRODO_AES_CTR_INIT(aes_ctx_ptr, key, iv)\
    AES_init(aes_ctx_ptr, CTR16, FRODO_AES_BLOCK_SIZE, (char*)key, (char*)iv);

#define FRODO_AES_CTR_ENCRYPT_BUFFER(aes_ctx_ptr, buf, buf_len)\
    for (int i = 0; i < buf_len; i+=FRODO_AES_BLOCK_SIZE)\
        AES_encrypt(aes_ctx_ptr, ((char*)buf)+i);

#define FRODO_AES_ECB_ENCRYPT_SINGLE(aes_ctx_ptr, buf)\
    AES_ecb_encrypt(aes_ctx_ptr, (uchar*)buf);

#define FRODO_AES_ECB_ENCRYPT_BUFFER(aes_ctx_ptr, buf, buf_len)\
    for (int i = 0; i < buf_len; i+=FRODO_AES_BLOCK_SIZE)\
        AES_ecb_encrypt(aes_ctx_ptr, ((uchar*)buf)+i);

#define FRODO_AES_CLEAN(aes_ctx_ptr)\
    AES_end(aes_ctx_ptr);

#elif FRODO_BACKEND == FRODO_BACKEND_OPENSSL

#include <inttypes.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

/* Workaround for conflicts with amcl */
/* Marsaglia & Zaman Random number generator constants */

#define NK 21 /**< PRNG constant */

typedef struct
{
    uint32_t ira[NK]; /**< random number array   */
    int      rndptr;  /**< pointer into array */
    uint32_t borrow;  /**<  borrow as a result of subtraction */
    int pool_ptr;     /**< pointer into random pool */
    char pool[32];    /**< random pool */
} csprng;

/* random numbers */
/**    @brief Seed a random number generator from an array of bytes
 *
    The provided seed should be truly random
    @param R an instance of a Cryptographically Secure Random Number Generator
    @param n the number of seed bytes provided
    @param b an array of seed bytes

 */
extern void RAND_seed(csprng *R,int n,char *b);

/**    @brief Delete all internal state of a random number generator
 *
    @param R an instance of a Cryptographically Secure Random Number Generator
 */
extern void RAND_clean(csprng *R);

/**    @brief Return a random byte from a random number generator
 *
    @param R an instance of a Cryptographically Secure Random Number Generator
    @return a random byte
 */
extern int RAND_byte(csprng *R);

/* Entropy generation macros */
#define FRODO_CSPRNG csprng

#define FRODO_CSPRNG_SEED(rng_ptr, seed) RAND_seed(rng_ptr, seed_len, (char*)seed);

#define FRODO_CSPRNG_GET(rng_ptr) RAND_byte(rng_ptr);

/* AES related macros */
#define FRODO_AES_CTX void*

#define FRODO_AES_ECB_INIT(aes_ctx_ptr, key)\
    *aes_ctx_ptr = EVP_CIPHER_CTX_new();\
    EVP_EncryptInit_ex(*aes_ctx_ptr, EVP_aes_128_ecb(), NULL, key, NULL);\
    EVP_CIPHER_CTX_set_padding(*aes_ctx_ptr, 0);

#define FRODO_AES_CTR_INIT(aes_ctx_ptr, key, iv)\
    *aes_ctx_ptr = EVP_CIPHER_CTX_new();\
    EVP_EncryptInit_ex(*aes_ctx_ptr, EVP_aes_128_ctr(), NULL, key, iv);

#define FRODO_AES_CTR_ENCRYPT_BUFFER(aes_ctx_ptr, buf, buf_len)\
    int tmp_buf_len;\
    EVP_EncryptUpdate(*aes_ctx_ptr, (unsigned char *)buf, &tmp_buf_len, (unsigned char *)buf, buf_len);\
    EVP_EncryptFinal_ex(*aes_ctx_ptr, (unsigned char *)buf, &tmp_buf_len);

#define FRODO_AES_ECB_ENCRYPT_SINGLE(aes_ctx_ptr, buf)\
    int tmp_buf_len;\
    EVP_EncryptUpdate(*aes_ctx_ptr, (unsigned char *)buf, &tmp_buf_len, (unsigned char *)buf, FRODO_AES_BLOCK_SIZE);\
    EVP_EncryptFinal_ex(*aes_ctx_ptr, (unsigned char *)buf, &tmp_buf_len);

#define FRODO_AES_ECB_ENCRYPT_BUFFER(aes_ctx_ptr, buf, buf_len)\
    int tmp_buf_len;\
    EVP_EncryptUpdate(*aes_ctx_ptr, (unsigned char *)buf, &tmp_buf_len, (unsigned char *)buf, buf_len);\
    EVP_EncryptFinal_ex(*aes_ctx_ptr, (unsigned char *)buf, &tmp_buf_len);

#define FRODO_AES_CLEAN(aes_ctx_ptr)\
    EVP_CIPHER_CTX_free(*aes_ctx_ptr);

#endif

#endif
