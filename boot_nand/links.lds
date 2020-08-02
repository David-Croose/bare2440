OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS
{ 
	firtst 0x00000000 : 
  	{
 	  	*(.boot.1)
 	  	*(.boot.2)
  		__stepping_stone_end = .;
  	}

  	second 0x30100000 : AT(4096)
  	{
  		. = ALIGN(4);
  		__image_start = .;
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

