#!/bin/bash

if [ $1 == "nor" ]; then
    echo "configure system booting from nor flash. the entry address is 0x30080000"
    cp -f boot_nor/boot.h 1_level/boot.h
    cp -f boot_nor/links.lds links.lds
    cp -f boot_nor/Makefile Makefile
    cp -f boot_nor/s3c2440_startup.S 3_level/arm920t/s3c2440/s3c2440_startup.S
else
    echo "configure system booting from nand flash. the entry address is 0x30100000"
    cp -f boot_nand/boot.h 1_level/boot.h
    cp -f boot_nand/links.lds links.lds
    cp -f boot_nand/Makefile Makefile
    cp -f boot_nand/s3c2440_startup.S 3_level/arm920t/s3c2440/s3c2440_startup.S
fi

make CROSS_COMPILER=/home/sam/Work/arm-linux-gcc-3.4.5/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-

