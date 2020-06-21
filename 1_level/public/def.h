#ifndef _DEF_H_
#define _DEF_H_

typedef unsigned char  u8;
typedef signed   char  s8;
typedef unsigned short u16;
typedef signed   short s16;
typedef unsigned int   u32;
typedef signed   int   s32;
typedef volatile unsigned char  vu8;
typedef volatile signed   char  vs8;
typedef volatile unsigned short vu16;
typedef volatile signed   short vs16;
typedef volatile unsigned int   vu32;
typedef volatile signed   int   vs32;

#ifndef bool
typedef enum { FALSE = 0, TRUE = 1 } bool;
#endif

#ifndef NULL
#define NULL    (void *)0
#endif

#endif
