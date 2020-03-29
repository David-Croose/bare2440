#!/bin/bash

if [ $# -ne 1 ]; then
    echo "wrong parameter! usage: ./compile.sh [nor] | [nan]"
    exit 1
fi

if [ $1 == "nor" ]; then
    echo "configure system booting from nor flash. the entry address is 0x30080000"
    cp -f boot_nor/boot.h 1_level/boot.h
    cp -f boot_nor/links.lds links.lds
    cp -f boot_nor/Makefile Makefile
    cp -f boot_nor/s3c2440_startup.S 3_level/arm920t/s3c2440/s3c2440_startup.S
elif [ $1 == "nan" ]; then
    echo "configure system booting from nand flash. the entry address is 0x30100000"
    cp -f boot_nand/boot.h 1_level/boot.h
    cp -f boot_nand/links.lds links.lds
    cp -f boot_nand/Makefile Makefile
    cp -f boot_nand/s3c2440_startup.S 3_level/arm920t/s3c2440/s3c2440_startup.S
else
    echo "wrong parameter! usage: ./compile.sh [nor] | [nan]"
    exit 2
fi

make

