OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{ 
  	second 0x30080000 : AT(0)
  	{ 
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

