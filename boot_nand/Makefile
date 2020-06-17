#======================================================================
# specify the compiler and it's compile options
CROSS_COMPILER ?= arm-linux-
CFLAGS = -Wall -O0 -g -std=c99

# target files, remember:the startup.S(containing the entry function of 
# the project) must in the first place!
TARGET = bare2440_boot_from_nand
SRC = ./3_level/arm920t/s3c2440/s3c2440_startup.S \
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
	  ./3_level/bsp/nand/nand_boot.c \
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

INC = -I./1_level \
      -I./1_level/public \
      -I./2_level/printf \
      -I./2_level/shell \
      -I./3_level/arm920t/s3c2440 \
      -I./3_level/bsp/gpio \
      -I./3_level/bsp/uart \
      -I./3_level/bsp/nand \
      -I./3_level/bsp/timer \
      -I./3_level/bsp/lcd \
	  -I./3_level/bsp/tinyusb/host/ehci/ \
	  -I./3_level/bsp/tinyusb/host/ohci/ \
	  -I./3_level/bsp/tinyusb/host/ \
	  -I./3_level/bsp/tinyusb/class/usbtmc/ \
	  -I./3_level/bsp/tinyusb/class/net/ \
	  -I./3_level/bsp/tinyusb/class/audio/ \
	  -I./3_level/bsp/tinyusb/class/bth/ \
	  -I./3_level/bsp/tinyusb/class/vendor/ \
	  -I./3_level/bsp/tinyusb/class/hid/ \
	  -I./3_level/bsp/tinyusb/class/dfu/ \
	  -I./3_level/bsp/tinyusb/class/midi/ \
	  -I./3_level/bsp/tinyusb/class/cdc/ \
	  -I./3_level/bsp/tinyusb/class/msc/ \
	  -I./3_level/bsp/tinyusb/device/ \
	  -I./3_level/bsp/tinyusb/osal/ \
	  -I./3_level/bsp/tinyusb/ \
	  -I./3_level/bsp/tinyusb/common/

# you may need to add some archive lib here
SLIB =	`$(CC) -print-file-name=libc.a` \
        `$(CC) -print-file-name=libgcc.a`

#----------------------------------------------------------------------
TARGET ?= target

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

$(TARGET) : $(OBJ)
	@echo "  Linking ..."
	@$(LD) -Tlinks.lds $(OBJ) $(SLIB) -o $(TARGET_ELF)
	@$(OBJCOPY) -O binary -S $(TARGET_ELF) $(TARGET_BIN)
	@$(OBJDUMP) -D -m arm $(TARGET_ELF) > $(TARGET_DIS)
	@echo "  "$(TARGET_BIN)" is ready"
	@echo "  end\n"

%.o : %.c
	@$(CC) $(CFLAGS) $(INC) -o $@ -c $<
	@echo "  CC    "$<

%.o : %.S
	@$(AS) $(CFLAGS) $(INC) -o $@ -c $<
	@echo "  AS    "$<

clean :
	@rm -f $(TARGET_ELF) $(TARGET_DIS) $(TARGET_BIN) $(OBJ)
	@echo "  clean over\n"

