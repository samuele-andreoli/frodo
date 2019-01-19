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

#include "frodo.h"

#define PRINT_BYTE_ARRAY(k, s) for(int i = 0; i < s; i++) printf("%02X", k[i]);

int main()
{
    csprng RNG;
    char seed[100];

    for (int i=0;i<100;i++)
        seed[i] = i;
    RAND_seed(&RNG,100,seed);

    printf("Test protocol run");
    FRODO_left_keypair lk = {0};
    FRODO_right_keypair rk = {0};

    // Generate seed
    uint8_t param_seed[FRODO_SEED_LENGTH] = {0};

    printf("Generate seed:\n");
    PRINT_BYTE_ARRAY(param_seed, FRODO_SEED_LENGTH);
    printf("\n");

    // Generate keys
    FRODO_generate_left_keypair(&RNG, &lk, param_seed);
    FRODO_generate_right_keypair(&RNG, &rk, param_seed);

    // Pack right key
    uint8_t right_keyshare[FRODO_PACKED_SHARE_LENGTH] = {0};

    FRODO_right_keyshare_pack(right_keyshare, &rk);

    // Perform left keyshare
    uint8_t key_left[FRODO_KEY_LENGTH] = {0};
    uint8_t left_keyshare[FRODO_PACKED_SHARE_LENGTH] = {0};
    uint8_t hint[FRODO_HINT_LENGTH] = {0};

    FRODO_left_key_agreement(&RNG, key_left, left_keyshare, hint, right_keyshare, &lk);

    // Perform right keyshare
    uint8_t key_right[FRODO_KEY_LENGTH] = {0};

    FRODO_right_key_agreement(key_right, left_keyshare, &rk, hint);

    printf("Right reconciled key\n");
    PRINT_BYTE_ARRAY(key_right, FRODO_KEY_LENGTH);
    printf("\n");

    printf("Mask\n");
    for(int i = 0; i < FRODO_KEY_LENGTH; i++)
    {
    	printf("%02X", key_left[i]^key_right[i]);
    }
    printf("\n");
}
