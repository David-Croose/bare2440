#ifndef _SHELL_H_XXX
#define _SHILL_H_XXX

#include "s3c2440.h"

typedef struct
{
	const char *cmd;
	void (*callback)(int argc, char *argv[]);
	const char *feature_and_usage;
}
cmd_t;

void shell_do(u8 *buf, u32 len);

#endif

