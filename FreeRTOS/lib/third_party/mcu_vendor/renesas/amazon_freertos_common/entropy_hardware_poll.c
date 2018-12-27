/**
  ******************************************************************************
  * @file    entropy_hardware_poll.c
  *
  ******************************************************************************
  */

#include <string.h>
#include "platform.h"   // __LIT for all compilers

int mbedtls_hardware_poll( void *data, unsigned char *output, size_t len, size_t *olen );
void get_random_number(uint8_t *data, uint32_t len);

int mbedtls_hardware_poll( void *data,
                    unsigned char *output, size_t len, size_t *olen )
{
	uint32_t random_number = 0;

	get_random_number(&random_number, sizeof(uint32_t));
	((void) data);
	*olen = 0;

	memcpy(output, &random_number, sizeof(uint32_t));
	*olen = sizeof(uint32_t);

	return 0;
}

/******************************************************************************
Functions : random number generator(XorShift method)
******************************************************************************/
void get_random_number(uint8_t *data, uint32_t len)
{
    static uint32_t y = 2463534242;
    uint32_t res;
    uint32_t lp;
    uint8_t *bPtr;

    res = len / 4;
    for (lp = 0; lp < res; lp++)
    {
        y = y ^ (y << 13);
        y = y ^ (y >> 17);
        y = y ^ (y << 5);
        bPtr = (uint8_t*) & y;
#if __LIT
        *((uint32_t *)data) = (*(bPtr + 3) << 24) | (*(bPtr + 2) << 16) | (*(bPtr + 1) << 8) | *(bPtr + 0);
#else
        *((uint32_t *)data) = y;
#endif
        data += 4;
    }
    y = y ^ (y << 13);
    y = y ^ (y >> 17);
    y = y ^ (y << 5);
    res = (uint32_t)len % 4;
    bPtr = (uint8_t*) & y;
    switch (res)
    {
        case 3:
#if __LIT
            *data++ = bPtr[3];
            *data++ = bPtr[2];
            *data++ = bPtr[1];
#else
            *data++ = bPtr[0];
            *data++ = bPtr[1];
            *data++ = bPtr[2];
#endif
            break;

        case 2:
#if __LIT
            *data++ = bPtr[3];
            *data++ = bPtr[2];
#else
            *data++ = bPtr[0];
            *data++ = bPtr[1];
#endif
            break;

        case 1:
#if __LIT
            *data++ = bPtr[3];
#else
            *data++ = bPtr[0];
#endif
            break;

        default:
            /* no op */
            break;
    }
}
