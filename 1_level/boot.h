#define BOOTING_FROM_NORFLASH

extern unsigned int __new_vector_start;
extern unsigned int __new_vector_end;

static void move_program(void)
{
    volatile unsigned int *d, *s;

    /*
     * it won't work if you use memcpy here
     */
    for (d = (volatile unsigned int *)0x30100000, s = &__new_vector_start;
         s < &__new_vector_end; d++, s++) {
        *d = *s;
    }
}

