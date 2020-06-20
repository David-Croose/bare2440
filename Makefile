CROSS_COMPILER ?= arm-linux-
CFLAGS = -Wall -O0 -g -std=c99
BOOT ?= nand
EMPTY ?=

ifeq ($(BOOT), nand)
	BOOT_DIR = boot_nand
	TARGET = bare2440_nand_boot
	ENTRYADDR = 0x0(SRAM) and 0x30100000(SDRAM)
	NANDBOOTSRC = 3_level/bsp/nand/nand_boot.c
else
	BOOT_DIR = boot_nor
	TARGET = bare2440_nor_boot
	ENTRYADDR = 0x30080000(SDRAM)
	NANDBOOTSRC = $(EMPTY)
	CFLAGS += -DBOOTING_FROM_NORFLASH
endif

# target files, remember:the startup.S(containing the entry function of
# the project) must in the first place!
SRC = BOOT_DIR/s3c2440_startup.S
SRC += $(NANDBOOTSRC)
SRC += \
	./3_level/arm920t/s3c2440/s3c2440.c \
	./3_level/arm920t/s3c2440/s3c2440_it.c \
	./3_level/arm920t/s3c2440/s3c2440_misc.S \
	./3_level/arm920t/s3c2440/s3c2440_new_vector.S \
	./3_level/bsp/gpio/led.c \
	./3_level/bsp/gpio/key.c \
	./3_level/bsp/uart/uart0.c \
	./3_level/bsp/timer/timer0.c \
	./3_level/bsp/timer/timer4.c \
	./3_level/bsp/nand/nand.c \
	./3_level/bsp/lcd/lcd.c \
	./3_level/bsp/tinyusb/host/usbh.c \
	./3_level/bsp/tinyusb/host/ohci/ohci.c \
	./3_level/bsp/tinyusb/host/hub.c \
	./3_level/bsp/tinyusb/class/usbtmc/usbtmc_device.c \
	./3_level/bsp/tinyusb/class/net/net_device.c \
	./3_level/bsp/tinyusb/class/bth/bth_device.c \
	./3_level/bsp/tinyusb/class/vendor/vendor_host.c \
	./3_level/bsp/tinyusb/class/vendor/vendor_device.c \
	./3_level/bsp/tinyusb/class/hid/hid_host.c \
	./3_level/bsp/tinyusb/class/hid/hid_device.c \
	./3_level/bsp/tinyusb/class/dfu/dfu_rt_device.c \
	./3_level/bsp/tinyusb/class/midi/midi_device.c \
	./3_level/bsp/tinyusb/class/cdc/cdc_device.c \
	./3_level/bsp/tinyusb/class/cdc/cdc_rndis_host.c \
	./3_level/bsp/tinyusb/class/cdc/cdc_host.c \
	./3_level/bsp/tinyusb/class/msc/msc_device.c \
	./3_level/bsp/tinyusb/class/msc/msc_host.c \
	./3_level/bsp/tinyusb/device/usbd.c \
	./3_level/bsp/tinyusb/device/usbd_control.c \
	./3_level/bsp/tinyusb/tusb.c \
	./3_level/bsp/tinyusb/common/tusb_fifo.c \
	./1_level/main.c \
	./1_level/public/clib.c \
	./1_level/public/crc.c \
	./1_level/public/delay.c \
	./2_level/printf/printf.c \
	./2_level/shell/shell.c \

INC = \
	-I./1_level \
	-I./1_level/public \
	-I./2_level/printf \
	-I./2_level/shell \
	-I./3_level/arm920t/s3c2440 \
	-I./3_level/bsp/gpio \
	-I./3_level/bsp/uart \
	-I./3_level/bsp/nand \
	-I./3_level/bsp/timer \
	-I./3_level/bsp/lcd \
	-I./3_level/bsp/tinyusb/host/ehci \
	-I./3_level/bsp/tinyusb/host/ohci \
	-I./3_level/bsp/tinyusb/host \
	-I./3_level/bsp/tinyusb/class/usbtmc \
	-I./3_level/bsp/tinyusb/class/net \
	-I./3_level/bsp/tinyusb/class/audio \
	-I./3_level/bsp/tinyusb/class/bth \
	-I./3_level/bsp/tinyusb/class/vendor \
	-I./3_level/bsp/tinyusb/class/hid \
	-I./3_level/bsp/tinyusb/class/dfu \
	-I./3_level/bsp/tinyusb/class/midi \
	-I./3_level/bsp/tinyusb/class/cdc \
	-I./3_level/bsp/tinyusb/class/msc \
	-I./3_level/bsp/tinyusb/device \
	-I./3_level/bsp/tinyusb/osal \
	-I./3_level/bsp/tinyusb \
	-I./3_level/bsp/tinyusb/common

# you may need to add some archive lib here
SLIB = \
	`$(CC) -print-file-name=libc.a` \
	`$(CC) -print-file-name=libgcc.a`

AS      = $(CROSS_COMPILER)gcc
CC      = $(CROSS_COMPILER)gcc
LD      = $(CROSS_COMPILER)ld
NM      = $(CROSS_COMPILER)nm
SIZE    = $(CROSS_COMPILER)size
OBJCOPY = $(CROSS_COMPILER)objcopy
OBJDUMP = $(CROSS_COMPILER)objdump

TARGET_ELF = $(TARGET).elf
TARGET_DIS = $(TARGET).dis
TARGET_BIN = $(TARGET).bin

OBJ = $(patsubst %.c, %.o, $(patsubst %.S, %.o, $(SRC)))

$(TARGET): $(OBJ) Makefile howtoburn
	@$(LD) -T$(BOOT_DIR)links.lds $(OBJ) $(SLIB) -o $(TARGET_ELF)
	@$(OBJCOPY) -O binary -S $(TARGET_ELF) $(TARGET_BIN)
	@$(OBJDUMP) -D -m arm $(TARGET_ELF) > $(TARGET_DIS)
	@echo "  $(TARGET_BIN) is ready, it's entry address is $(ENTRYADDR)"

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<
	@echo "  CC    "$<

%.o: %.S
	@$(AS) $(CFLAGS) $(INC) -o $@ -c $<
	@echo "  AS    "$<

howtoburn:
	@echo "how to burn the image:"
ifeq ($(BOOT), nand)
	@echo " 	1. push the board's boot switch button to nor boot then power up."
	@echo "		2. enter backspace to let u-boot stay in command line."
    @echo "		3. enter 'n' to u-boot menu in your serial terminal."
    @echo "		4. start dnw, USB Port--->Transmit, don't need to care about the Configuration."
	@echo " 	5. push the board's boot switch button to nand boot then repower up. you may need to wait for a while for the board's booting."
	@echo "		6. done."
else
	@echo " 	1. push the board's boot switch button to nor boot then power up."
	@echo "		2. enter backspace to let u-boot stay in command line."
    @echo "		3. enter 'd' to u-boot menu in your serial terminal."
    @echo "		4. start dnw, Configuration--->Options--->Download Address 0x30080000, USB Port--->Transmit."
	@echo "		5. done."
endif
	@echo "prerequisite:"
	@echo "		* the u-boot firmware(can be found in 'tools/dnw/u-boot.bin') in norflash. if negative, you may need to burn it into your board by OpenJTAG+OpenOCD."
	@echo "		* dnw, windows version."
	@echo "		* a serial terminal."
	@echo
	@echo "note:"
	@echo "		* it's forbidden to set the entry address to 0x30000000 in the link script, cause that's the MMU memory map table."
	@echo "		* you may need to change the copy size(a parameter of nand_boot_copy2sdram) of nand flash image when your image which booting from nand is too large."
	@echo

clean:
	@rm -f $(TARGET_ELF) $(TARGET_DIS) $(TARGET_BIN) $(OBJ)

help:
	@echo "Usage: make [BOOT=nand|nor] [CROSS_COMPILER=<CROSS_COMPILER's prefix] [clean] [help]"
	@echo "		BOOT				the booting flash, default: nand."
	@echo "		CROSS_COMPILER		the prefix of user's cross-compier. default: 'arm-linux-'."
	@echo "		clean				clean all files generated by buid."
	@echo "		help				print help message."


.PHONY: clean help howtoburn
