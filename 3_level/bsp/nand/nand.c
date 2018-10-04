/**
 * nand flash driver for Samsung K9F2G08U0C
 *
 * size : 256MB
 * block total : 2048
 * block size  : 128KB
 * how many pages per block : 64
 * how many bytes per page  : 2048KB
 * how many bytes per spare of page : 64B
 */
#include "nand.h"

void nand_clock_init(void)
{
	u8 p1, p2, p3;
#if (CONFIG_S3C2440_MPLL_CLK == S3C2440_MPLL_CLK_200MHZ)
	p1 = 0;
	p2 = 3;
	p3 = 0;
#elif (CONFIG_S3C2440_MPLL_CLK == S3C2440_MPLL_CLK_400MHZ)
	p1 = 3 - 1;
	p2 = 7 - 1;
	p3 = 3 - 1;
#else
#error "the definition of S3C2440_MPLL_CLK is wrong !"
#endif
	*(vu32 *)NFCONF = (p1 << 12) | (p2 << 8) | (p3 << 4);
}

/**
 * once you lock, the lock can never be unlock until the chip reset
 */
void nand_lock_tight(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1 << 13;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 13);
		*(vu32 *)NFCONT = tmp;
	}
}

/**
 * Soft lock area can be modified at any time by software
 */
void nand_soft_lock(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1 << 12;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 12);
		*(vu32 *)NFCONT = tmp;
	}
}

void nand_illegal_access_irq_en(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1 << 10;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 10);
		*(vu32 *)NFCONT = tmp;
	}
}

/**
 * enable or disable the RnB(ready and busy) irq
 */
void nand_rnb_irq_en(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1 << 9;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 9);
		*(vu32 *)NFCONT = tmp;
	}
}

/**
 * set tht RnB(ready and busy) pin's transportation mode
 * @param mode 0 --- rising
 *             1 --- falling
 */
void nand_rnb_trans_mode(u32 mode)
{
	u32 tmp;

	if(mode == 1)
	{
		*(vu32 *)NFCONT |= 1 << 8;
	}
	else
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 8);
		*(vu32 *)NFCONT = tmp;
	}
}

void nand_spare_ecc_lock(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1 << 6;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 6);
		*(vu32 *)NFCONT = tmp;
	}
}

void nand_main_ecc_lock(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1 << 5;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 5);
		*(vu32 *)NFCONT = tmp;
	}
}

/**
 * Initialize ECC decoder/encoder(Write-only)
 */
void nand_init_ecc(void)
{
	*(vu32 *)NFCONT |= 1 << 4;
}

void nand_enable(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		*(vu32 *)NFCONT |= 1;
	}
	else if(flag == FALSE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~1;
		*(vu32 *)NFCONT = tmp;
	}
}

void nand_select(bool flag)
{
	u32 tmp;

	if(flag == TRUE)
	{
		tmp = *(vu32 *)NFCONT;
		tmp &= ~(1 << 1);
		*(vu32 *)NFCONT = tmp;
	}
	else if(flag == FALSE)
	{
		*(vu32 *)NFCONT |= 1 << 1;
	}
}

void nand_send_cmd(u8 cmd)
{
	*(vu32 *)NFCMD = cmd;
}

void nand_send_addr(u32 addr)
{
	u32 i, col, page;
	vu8 *p = (vu8 *)NFADDR;

	col = addr & ((2048 + 64) - 1);
	page = addr / (2048 + 64);

	*p = col & 0xff;              /* Column Address A0~A7 */
	for(i = 0; i < 10; i ++);
	*p = (col >> 8) & 0x0f;       /* Column Address A8~A11 */
	for(i = 0; i < 10; i ++);
	*p = page & 0xff;             /* Row Address A12~A19 */
	for(i = 0; i < 10; i ++);
	*p = (page >> 8) & 0xff;      /* Row Address A20~A27 */
	for(i = 0; i < 10; i ++);
	*p = (page >> 16) & 0x03;     /* Row Address A28~A29 */
	for(i = 0; i < 10; i ++);
}

void nand_send_block_addr(u32 block_addr)
{
	u32 i, page;
	vu8 *p = (vu8 *)NFADDR;

	page = block_addr / (2048 + 64);

	*p = page & 0xff;             /* Row Address A12~A19 */
	for(i = 0; i < 10; i ++);
	*p = (page >> 8) & 0xff;      /* Row Address A20~A27 */
	for(i = 0; i < 10; i ++);
	*p = (page >> 16) & 0x03;     /* Row Address A28~A29 */
	for(i = 0; i < 10; i ++);
}

void nand_write_data(u8 data)
{
	*(vu8 *)NFDATA = data;
}

u8 nand_read_data(void)
{
	return *(vu8 *)NFDATA;
}

bool nand_is_busy(void)
{
	u32 tmp;

	tmp = *(vu32 *)NFSTAT;
	if(tmp & 1)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

bool nand_is_access_illegal(void)
{
	u32 tmp;

	tmp = *(vu32 *)NFSTAT;
	if(tmp & (1 << 3))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

bool nand_is_rnb_trans_detect(void)
{
	u32 tmp;

	tmp = *(vu32 *)NFSTAT;
	if(tmp & (1 << 2))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void nand_set_start_block_addr(void)
{
	*(vu32 *)NFSBLK = 0;
}

void nand_set_end_block_addr(void)
{
	*(vu32 *)NFEBLK = 0xFFFFFFFF;
}

void nand_init(void)
{
	clock_enable(CLKSRC_NAND);
	nand_clock_init();
	nand_enable(TRUE);
	nand_select(FALSE);
	nand_send_cmd(0xFF);    // reset the NandFlash
//	nand_lock_tight(FALSE);
//	nand_soft_lock(FALSE);
//	nand_illegal_access_irq_en(FALSE);
//	nand_rnb_trans_mode(0);
//	nand_spare_ecc_lock(FALSE);
//	nand_init_ecc();
//	nand_set_start_block_addr();
//	nand_set_end_block_addr();
}

/**
 * write one page to nand flash
 * @param addr : the nandflash address
 * @param buf  : the buffer in sdram or sram to write to
 * @param bytes : how many bytes(<2048) to write
 * @return     : TRUE  --- write successfully
 *               FALSE --- write failed
 */
bool nand_write(u32 addr, const u8 *buf, u32 bytes)
{
	u32 i, j = 0;
	bool ret = FALSE;

	nand_select(TRUE);

	nand_send_cmd(0x80);
	nand_send_addr(addr);
	for(i = 0; i < bytes; i ++)
	{
		nand_write_data(buf[j ++]);
	}
	nand_send_cmd(0x10);
	while(nand_is_busy());
	
	if(nand_read_data() == 0)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}

	nand_select(FALSE);
	return ret;
}

/**
 * read one page from nand flash
 * @param addr : the nandflash address
 * @param buf  : the buffer in sdram or sram to read to
 * @param bytes : how many bytes(<2048) to read
 */
void nand_read(u32 addr, u8 *buf, u32 bytes)
{
	u32 i, j = 0;

	nand_select(TRUE);
	nand_send_cmd(0);
	nand_send_addr(addr);
	nand_send_cmd(0x30);
	while(nand_is_busy());

	for(i = 0; i < bytes; i ++)
	{
		buf[j ++] = nand_read_data();
	}

	nand_select(FALSE);
}

/**
 * nand erase block
 * @param block_addr : the block address you want to erase,
 *                     like : 0 * 128 * 1024(the 0th blcok address); 2 * 128 * 1024(the 2th blcok address)
 * @retval : TRUE --- success
 *           FALSE --- FAILED
 */
bool nand_erase_block(u32 block_addr)
{
	bool ret = FALSE;

	nand_select(TRUE);

	nand_send_cmd(0x60);
	nand_send_block_addr(block_addr);
	nand_send_cmd(0xD0);
	while(nand_is_busy());
	nand_send_cmd(0x70);

	if((nand_read_data() & 0x01) == 0)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}

	nand_select(FALSE);
	return ret;
}

