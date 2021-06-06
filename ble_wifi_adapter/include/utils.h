
#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t get_CRC32(uint8_t *data, int16_t len);

#ifdef __cplusplus
}
#endif

#endif //__UTILS_H