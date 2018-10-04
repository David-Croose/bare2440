OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{ 
	firtst 0x00000000 : 
  	{
		./3_level/arm920t/s3c2440/s3c2440_startup.o 
		./3_level/bsp/nand/nand_boot.o
  		__stepping_stone_end = .;
  	}

  	second 0x30100000 : AT(4096)
  	{ 
 	  	*(.new_vector)
   	  	*(.text)
  	  	*(.rodata*)
  	  	*(.data)
  	  	*(.got*)
		*(.gnu.linkonce*)
  	}

   	.bss : 
  	{
  	    . = ALIGN(4);
  	    __bss_start = .; 
  	    *(.bss)
  	    *(COMMON)
  	    . = ALIGN(4);
  	    __bss_end = .;
  	}
}

