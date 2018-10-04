/**
 * Author:Sam Hotiky
 * Description:nandflash register and other stuffs
 */

#ifndef _NAND_H_
#define _NAND_H_

#include "s3c2440.h"

// #define TACLS   0
// #define TWRPH0  3
// #define TWRPH1  0
#define TACLS   2
#define TWRPH0  6
#define TWRPH1  2

void nand_init(void);
bool nand_write(u32 addr, const u8 *buf, u32 bytes);
void nand_read(u32 addr, u8 *buf, u32 bytes);
bool nand_erase_block(u32 block_addr);

#endif
