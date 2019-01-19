#include "amcl.h"
#include "packing.h"

#define BTW_CMP(a,b) (((a) ^ (b)) & mask) != 0

#define PRINT_BYTE_ARRAY(k, s) for(int i = 0; i < s; i++) printf("%04X", k[i]);


const uint16_t mask = (1 << FRODO_MODULUS) - 1;

int main()
{   
    csprng RNG;
    char rng_seed[100];

    uint16_t share[FRODO_N * FRODO_BAR_N] = {0};
    uint8_t packed[FRODO_PACKED_SHARE_LENGTH] = {0};
    uint16_t recovered_share[FRODO_N * FRODO_BAR_N] = {0};

    for (int i=0; i<100; i++)
    {
        rng_seed[i] = i;
    }
    RAND_seed(&RNG, 100, rng_seed);

    for(int i = 0; i < FRODO_N * FRODO_BAR_N; i++)
    {
        share[i] = ((uint16_t)RAND_byte(&RNG) << 8) | ((uint16_t)RAND_byte(&RNG));
    }

    FRODO_pack_share(packed, share);
    FRODO_unpack_share(recovered_share, packed);

    for(int i=0; i<FRODO_N * FRODO_BAR_N; i++)
    {
        if(BTW_CMP(share[i], recovered_share[i]))
        {
            return 1;
        }
    }

    return 0;
}