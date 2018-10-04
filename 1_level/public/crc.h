#ifndef _CRC_H_
#define _CRC_H_

#include "def.h"

u8 crc7(u8 crc, const u8 *buffer, u32 len);
u16 crc16(u16 crc, u8 const *buffer, u32 len);

#endif
