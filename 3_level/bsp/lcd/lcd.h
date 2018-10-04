#ifndef _LCD_H_
#define _LCD_H_

#include "s3c2440.h"

#define LCD_X_RES    480						// LCD panel horizontal resolution
#define LCD_Y_RES    272						// LCD panel vertical resolution
#define LCD_FB_ADDR  (0x30000000 + 0x400000)	// LCD frame buffer base address

void lcd_enable(void);
void lcd_disable(void);
void lcd_init(void);
void lcd_draw_point(u32 x, u32 y, u16 color);
u16 lcd_read_point(u32 x, u32 y);
void lcd_fill_rect(u32 x0, u32 y0, u32 x1, u32 y1, u16 color);
void lcd_clear(u16 color);

#endif
