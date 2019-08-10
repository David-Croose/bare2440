OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{ 
  	second 0x30080000 : AT(0)
  	{ 
		./3_level/arm920t/s3c2440/s3c2440_startup.o 
   	  	*(.text)
  	  	*(.rodata*)
  	  	*(.data)
  	  	*(.got*)
		*(.gnu.linkonce*)

  	    . = ALIGN(4);
		__new_vector_start = .;
 	  	*(.new_vector)
		__new_vector_end = .;
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

