#include "lcd.h"

static void lcd_gpio_init(void)
{
    *(vu32 *)GPCUP   = 0xFFFFFFFF;
    *(vu32 *)GPCCON  = 0xAAAAAAAA;
    *(vu32 *)GPDUP   = 0xFFFFFFFF;
    *(vu32 *)GPDCON  = 0xAAAAAAAA;
  	*(vu32 *)GPBCON &= ~3;
    *(vu32 *)GPBCON |= 1;
    *(vu32 *)GPBDAT &= ~1;
}

static void lcd_reg_init(void)
{
#define LCDTYPE_TFT     0x3
#define BPPMODE_1BPP    0x8
#define BPPMODE_2BPP    0x9
#define BPPMODE_4BPP    0xA
#define BPPMODE_8BPP    0xB
#define BPPMODE_16BPP   0xC
#define BPPMODE_24BPP   0xD
#define ENVID_DISABLE   0
#define ENVID_ENABLE    1
#define FORMAT8BPP_5551 0
#define FORMAT8BPP_565  1
#define HSYNC_NORM      0
#define HSYNC_INV       1
#define VSYNC_NORM      0
#define VSYNC_INV       1
#define BSWP            1
#define HWSWP           1
#define LOWER21BITS(n)  ((n) & 0x1FFFFF)

	int invpwren = 0;		// LCD_PWREN's polarity
	int pwren = 1;			// LCD_PWREN output valid

	*(vu32 *)LCDCON1 = (4 << 8) | (LCDTYPE_TFT << 5) | (BPPMODE_16BPP << 1) | (ENVID_DISABLE << 0);
	*(vu32 *)LCDCON2 = (1 << 24) | (271 << 14) | (1 << 6) | (9);
	*(vu32 *)LCDCON3 = (1 << 19) | (479 << 8) | (1);
	*(vu32 *)LCDCON4 = 40;
	*(vu32 *)LCDCON5 = (FORMAT8BPP_565 << 11) | (HSYNC_INV << 9) | (VSYNC_INV << 8) | (HWSWP << 1);

	*(vu32 *)LCDSADDR1 = ((LCD_FB_ADDR >> 22) << 21) | LOWER21BITS(LCD_FB_ADDR >> 1);
	*(vu32 *)LCDSADDR2 = LOWER21BITS((LCD_FB_ADDR + (LCD_X_RES) * (LCD_Y_RES) * 2) >> 1);
	*(vu32 *)LCDSADDR3 = (0 << 11) | (LCD_X_RES * 2 / 2);

	*(vu32 *)TPAL = 0;

	*(vu32 *)GPGCON = (*(vu32 *)GPGCON & (~(3 << 8))) | (3 << 8);				// GPG4 is used for LCD_PWREN
    *(vu32 *)GPGUP  = (*(vu32 *)GPGUP & (~(1 << 4))) | (1 << 4);				// disable the pull-up resister

    *(vu32 *)LCDCON5 = (*(vu32 *)LCDCON5 & (~(1 << 5))) | (invpwren << 5);
    *(vu32 *)LCDCON5 = (*(vu32 *)LCDCON5 & (~(1 << 3))) | (pwren << 3);
}

void lcd_enable(void)
{
	*(vu32 *)LCDCON1 |= 1;            // ENVID on
	*(vu32 *)GPBDAT |= (1 << 0);      // Power on
}

void lcd_disable(void)
{
	*(vu32 *)LCDCON1 &= 0x3FFFE;      // ENVID Off
	*(vu32 *)GPBDAT &= ~(1 << 0);     // Power off
}

void lcd_init(void)
{
	clock_enable(CLKSRC_LCD);
	lcd_gpio_init();
	lcd_reg_init();
	lcd_enable();
}

void lcd_draw_point(u32 x, u32 y, u16 color)
{
	*(vu16 *)(LCD_FB_ADDR + y * LCD_X_RES + x) = color;
}

u16 lcd_read_point(u32 x, u32 y)
{
	return *(vu16 *)(LCD_FB_ADDR + y * LCD_X_RES + x);
}

void lcd_fill_rect(u32 x0, u32 y0, u32 x1, u32 y1, u16 color)
{
	u32 i, j;
	u32 x_sz = x1 - x0 + 1;

	for(j = y0; j < y1; j++)
	{
		for(i = x0; i < x1; i++)
		{
			*(vu16 *)(LCD_FB_ADDR + j * x_sz + i) = color;
		}
	}
}

void lcd_clear(u16 color)
{
	u32 x, y;

	for(y = 0; y < LCD_Y_RES; y++)
	{
		for(x = 0; x < LCD_X_RES; x++)
		{
			*(vu16 *)(LCD_FB_ADDR + y * LCD_X_RES + x) = color;
		}
	}
}

