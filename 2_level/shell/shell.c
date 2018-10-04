#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include "printf.h"
#include "nand.h"
#include "uart0.h"

#define MAX_ARGC        10
#define MAX_ARGV_LEN    100

// the tool function to be called by shell_do
//============================================================================================
static void list_all_cmd(int argc, char *argv[])
{
	extern const cmd_t cmd_table[]; 
	u32 i;

	my_printf("nand infomation :\r\n");
	my_printf("size : 256MB\r\n");
	my_printf("block total : 2048\r\n");
	my_printf("block size : (128 + 4)KB\r\n");
	my_printf("page size : (2048 + 64)B\r\n");

	for(i = 0; cmd_table[i].cmd != NULL; i++)
	{
		my_printf("%s --- %s\r\n", cmd_table[i].cmd, cmd_table[i].feature_and_usage);	
	}
}

static void print_page(u8 *buf)
{
	u8 tmp;
	u32 i;

	for(i = 0; i < 2048; i ++)
	{
		tmp = buf[i] >> 4;
		if(tmp < 10)
		{
			tmp += '0';
		}
		else
		{
			tmp -= 10;
			tmp += 'A';
		}
		my_printf("%c", tmp);

		tmp = buf[i] & 0xF;
		if(tmp < 10)
		{
			tmp += '0';
		}
		else
		{
			tmp -= 10;
			tmp += 'A';
		}
		my_printf("%c ", tmp);
	}
}

unsigned int my_pow(int x, int y)
{
	int _x = x;

	if(x < 0 || y < 0)
	{
		return 0;
	}
	if(y == 0)
	{
		return 1;
	}
	while(--y > 0)
	{
		x *= _x;
	}
	return x;
}

s32 my_atol(const char *num, bool *err)
{
	s32 ret = 0;
	u32 i, j, start = 0;
	s32 len;
	s32 positive = TRUE;

	if(num == NULL)
	{
		goto error;
	}
	len = strlen(num);
	if(len > 9)
	{
		len = 9;
	}
	if(len <= 0)
	{
		goto error;
	}
	if(num[0] == '-')
	{
		if(len < 2)
		{
			goto error;
		}
		positive = FALSE;
		start = 1;
	}
	else if(num[0] < '0' || num[0] > '9')
	{
		goto error;
	}
	for(i = start; i < len; i++)
	{
		if(num[i] < '0' || num[i] > '9')
		{
			goto error;
		}
		j = num[i] - '0';
		j *= my_pow(10, len - i - 1);
		ret += j;
	}
	if(positive == TRUE)
	{
		return ret;
	}
	else
	{
		return -ret;
	}

error:
	*err = TRUE;
	return 0;
}

/**
 * a nand flash erase, write, read test
 */
static void nandtest(int argc, char *argv[])
{
	s32 sblock, eblock;
	char wbuf[9];
	static u8 buf[2048];
	u32 i, j;
	bool err;

	my_printf("argc = %d, argv[1] = %s, argv[2] = %s\r\n", argc, argv[1], argv[2]);
	if(argc != 3)
	{
		my_printf("argc != 2 !\r\n", argc);
		return;
	}
	if(!argv[1] || !argv[2])
	{
		my_printf("!argv[1] || !argv[2]\r\n");
		return;
	}

	sblock = my_atol(argv[1], &err);
	if(err == TRUE)
	{
		my_printf("argv[1] error !\r\n");
		return;
	}
	eblock = my_atol(argv[2], &err);
	if(err == TRUE)
	{
		my_printf("argv[2] error !\r\n");
		return;
	}

//	sblock = strtol(argv[1], &p, 10);
//	eblock = strtol(argv[2], &p, 10);

	my_printf("sblock = %d, eblock = %d\r\n", sblock, eblock);

	if(sblock > eblock)
	{
		my_printf("sblock > eblock !\r\n");
		return;
	}
	if(eblock > 2048 - 1)
	{
		my_printf("eblock > 2048 - 1 !\r\n");
		return;
	}

	for(i = sblock; i <= eblock; i++)
	{
		my_printf("erasing block %d ...\r\n", i);
		nand_erase_block(i * (128 + 4) * 1024);
	}
	my_printf("\r\n");
	for(i = sblock; i <= eblock; i++)
	{
		my_printf("the first page of block %d :\r\n", i);
		nand_read(i * (128 + 4) * 1024, buf, 2048);
		print_page(buf);
		my_printf("\r\n");
	}
	my_printf("\r\n");
	for(i = sblock; i <= eblock; i++)
	{
		my_sprintf(wbuf, "abcdefg_%d", i % 10);
		my_printf("write \"%s\" to the first page of block %d\r\n", wbuf, i);
		nand_write(i * (128 + 4) * 1024, (u8 *)wbuf, 9);
	}
	my_printf("\r\n");
	for(i = sblock; i <= eblock; i++)
	{
		nand_read(i * (128 + 4) * 1024, buf, 9);
		my_printf("the first page of block %d : ", i);
		for(j = 0; j < 9; j++)
		{
			my_printf("%c", buf[j]);
		}
		my_printf("\r\n");
	}
}

static void do_nanderase(int argc, char *argv[])
{
	int addr;
	bool err;
	bool ret;

	if(argc != 2)
	{
		my_printf("argc != 2\r\n");
	}
	addr = my_atol(argv[1], &err);
	if(err == TRUE)
	{
		my_printf("parameter of addr error !\r\n");
	}

	ret = nand_erase_block(addr);
	if(ret == TRUE)
	{
		my_printf("nand erase successfully\r\n");
	}
	else
	{
		my_printf("nand erase failed\r\n");
	}
	return;
}

static void do_nanderase_chip(int argc, char *argv[])
{
	unsigned int i;
	bool ret;

	if(argc != 1)
	{
		my_printf("argc != 1\r\n");
	}

	for(i = 0; i < 2048; i++)
	{
		my_printf("erasing block %d ...\r\n", i);
		ret = nand_erase_block(i * (128 + 4) * 1024);
		if(ret != TRUE)
		{
			my_printf("bad block index(%d)\r\n", i);
		}
	}

	my_printf("erase whole chip done\r\n");
}

static void do_nandwrite(int argc, char *argv[])
{
	int addr;
	char *write_buf;
	int len;
	bool err;

	if(argc != 4)
	{
		my_printf("argc != 4\r\n");
		goto error;
	}
	addr = my_atol(argv[1], &err);
	if(err == TRUE)
	{
		goto error;
	}
	write_buf = argv[2];
	len = my_atol(argv[3], &err);
	if(err == TRUE)
	{
		goto error;
	}
	if(addr < 0 || addr > (256 * 1024 * 1024)
	   || write_buf == NULL
	   || len <= 0)
	{
		goto error;
	}

	nand_write(addr, (u8 *)write_buf, len);	
	my_printf("write nand successfully, addr = %d, write_buf = %s, len = %d\r\n", addr, write_buf, len);
	return;

error:
	my_printf("write nand error !\r\n");
	return;
}

static void do_nandread(int argc, char *argv[])
{
	int i;
	int addr;
	char read_buf[1000] = {0};
	int len;
	bool err;

	if(argc != 3)
	{
		my_printf("argc != 3\r\n");
		goto error;
	}
	addr = my_atol(argv[1], &err);
	if(err == TRUE)
	{
		goto error;
	}
	len = my_atol(argv[2], &err);
	if(err == TRUE)
	{
		goto error;
	}
	if(addr < 0 || addr > (256 * 1024 * 1024)
	   || len > 1000
	   || len <= 0)
	{
		goto error;
	}

	nand_read(addr, (u8 *)read_buf, len);	
	my_printf("read nand successfully, addr = %d, read_buf = %s, len = %d\r\n", addr, read_buf, len);
	my_printf("here is the hex show :\r\n");
	for(i = 0; i < len; i++)
	{
		my_printf("0x%02X \r\n", read_buf[i]);
	}

	return;

error:
	my_printf("read nand error !\r\n");
	return;
}

static void do_nand_scan_bbt(int argc, char *argv[])
{
	u32 i;
	u32 addr;
	u8 buf[6] = {0};

	my_printf("========= the bad block(s) =========\r\n");
	my_printf("index  user address    real address\r\n");

	// there are 2048 blocks in total
	for(i = 0; i < 2048; i++)
	{
		addr = i * (128 + 4) * 1024 + 2048;
		nand_read(addr, buf, 6);
		if(buf[5] != 0xFF)
		{
			my_printf("%d      0x%08X      0x%08X\r\n", i, i * 128 * 1024, i * (128 + 4) * 1024);
		}
	}

	my_printf("index : starts with 0\r\n");
	my_printf("user address : don't count the oob in\r\n");
	my_printf("real address : count the oob in\r\n");
}

//--------------------------------------------------------------------------------------------

const cmd_t cmd_table[] =
{
	{"help",           list_all_cmd,      "feature : list all cmd; usage : help"},
	{"nandtest",       nandtest,          "feature : nand read, write, erase test; usage : nandtest 20 25"},
	{"nanderase",      do_nanderase,      "feature : nand erase; usage : nanderase 135168"},
	{"nandwrite",      do_nandwrite,      "feature : nand write; usage : nandwrite 135168 hello 5"},
	{"nandread",       do_nandread,       "feature : nand read; usage : nandread 135168 5"},
	{"nand_scan_bbt",  do_nand_scan_bbt,  "feature : nand scan bbt; usage : nand_scan_bbt"}, 
	{"nanderase_chip", do_nanderase_chip, "feature : nand erase whole chip; usage : nanderase_chip"},
	{NULL,             NULL,              NULL},
};

static void str2arg(int *argc, char **argv_table, const char *str, u32 len)
{
	u32 i, j;
	char (*_argv)[MAX_ARGV_LEN] = (char(*)[MAX_ARGV_LEN])argv_table;
	u32 start, end, single_cmd_len, cmd_seq = 0/* command sequence */;

	for(i = 0; i < len;)
	{
		if(str[i] == ' ')
		{
			i++;
			continue;
		}

		start = i;
		for(j = i + 1; j < len; j++)
		{
			if(str[j] == ' ')
			{
				break;
			}
		}
		end = j;
		single_cmd_len = end - start;
		if(single_cmd_len > MAX_ARGV_LEN - 1)
		{
			single_cmd_len = MAX_ARGV_LEN - 1;
		}
		memcpy(_argv[cmd_seq], &str[start], single_cmd_len);
		_argv[cmd_seq][single_cmd_len] = '\0';
		cmd_seq++;
		i = end;
	}
	*argc = cmd_seq;
}

void shell_do(u8 *buf, u32 len)
{
	u32 i;
	char argv_table[MAX_ARGC][MAX_ARGV_LEN];
	char *argv[MAX_ARGV_LEN] = {NULL};
	int argc;
	
	memset(argv_table, 0, MAX_ARGC * MAX_ARGV_LEN);
	
	// format your buffer
	len -= 2;
	str2arg(&argc, (char **)argv_table, (char *)buf, len);
	if(argc > MAX_ARGC)
	{
		argc = MAX_ARGC;
	}
	for(i = 0; i < argc; i++)
	{
		argv[i] = argv_table[i];
	}
	// my_printf("buf = %s, len = %d, argc = %d\r\n", buf, len, argc);
	my_printf("argc = %d\r\n", argc);
	for(i = 0; i < argc; i++)
	{
		my_printf("argv[%d] = %s\r\n", i, argv[i]);
	}
	// acting your command
	for(i = 0; cmd_table[i].cmd != NULL; i++)
	{
		if(strcmp(cmd_table[i].cmd, argv[0]) == 0)    // if find your cmd in table
		{
			if(cmd_table[i].callback != NULL)
			{
				my_printf("now executing the cmd : %s\r\n", cmd_table[i].cmd);
				cmd_table[i].callback(argc, (char **)argv);
			}
			break;
		}
	}
}

