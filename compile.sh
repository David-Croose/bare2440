#!/bin/bash

if [ $# -ne 2 ]; then
    echo "wrong parameter! usage: ./compile.sh nor|nand arm-linux-"
    exit 1
fi

echo "you choose booting from: $1, CROSS_COMPILER is: $2"
rm -f 1_level/boot.h
rm -f links.lds
rm -f Makefile
rm -f 3_level/arm920t/s3c2440/s3c2440_startup.S

if [ $1 == "nor" ]; then
    echo "configure system booting from norflash. the entry address is 0x30080000."
    echo "note: it's unwise to set the entry address to 0x30000000, cause that's the MMU memory map table."
    echo "how to burn it:"
    echo "  1. press 'd' in u-boot menu(the u-boot is in the norflash, you can never write the norflash)."
    echo "  2. start dnw, Configuration--->Options--->Download Address 0x30080000, USB Port--->Transmit."
    echo "  3. input help to get the features."
    cp -f boot_nor/boot.h 1_level/boot.h
    cp -f boot_nor/links.lds links.lds
    cp -f boot_nor/Makefile Makefile
    cp -f boot_nor/s3c2440_startup.S 3_level/arm920t/s3c2440/s3c2440_startup.S
elif [ $1 == "nand" ]; then
    echo "configure system booting from nandflash. the entry address is 0x30100000."
    echo "note: it's unwise to set the entry address to 0x30000000, cause that's the MMU memory map table."
    echo "how to burn it:"
    echo "  1. press 'n' in u-boot menu(the u-boot is in the norflash, you can never write the norflash)."
    echo "  2. start dnw, USB Port--->Transmit, don't need to care about the Configuration."
    echo "  3. power down."
    echo "  4. make the switch button to nand boot."
    echo "  5. power up, then input help to get the features."
    cp -f boot_nand/boot.h 1_level/boot.h
    cp -f boot_nand/links.lds links.lds
    cp -f boot_nand/Makefile Makefile
    cp -f boot_nand/s3c2440_startup.S 3_level/arm920t/s3c2440/s3c2440_startup.S
else
    echo "wrong parameter! usage: ./compile.sh nor|nand arm-linux-"
    exit 2
fi

make CROSS_COMPILER=$2

