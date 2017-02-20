TARGET     = mil9x
OPTIMIZATION = 3


CROSS = arm-none-eabi-
CC = $(CROSS)gcc
CXX = $(CROSS)g++
LD = $(CROSS)ld
AR = $(CROSS)ar
AS = $(CROSS)gcc
GASP = $(CROSS)gasp
NM = $(CROSS)nm
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
RANLIB = $(CROSS)ranlib
STRIP = $(CROSS)strip
SIZE = $(CROSS)size
READELF = $(CROSS)readelf
MAKETXT = srec_cat
MKDIR = mkdir
CP = cp -p
RM = rm
MV = mv

MCU_NAME	= MDR1986VE91T
MCU_FAMILY	= MDR1986VE9x

MCU			= -mcpu=cortex-m3 -mthumb
MCU_CC		= $(MCU) -D__thumb2__=1 -mtune=cortex-m3 -msoft-float -mapcs-frame

BUILD_DIR	:= build
SRC_DIR		:= src
#LD_SCRIPT	:= $(MCU_NAME).S

DEFINE		:= $(MCU_FAMILY) $(MCU_NAME) HSE_VALUE=8000000 USE_$(MCU_FAMILY)


SPL_DIR		:= MDR32F9Qx_StdPeriph_Driver
CMSIS_DIR	:= CMSIS/CM3
USB_DIR		:= USB
SPL_SRC		:= $(wildcard $(SPL_DIR)/src/*.c)
SPL_INC		:= $(SPL_DIR)/inc
CMSIS_SRC	:= $(wildcard $(CMSIS_DIR)/CoreSupport/*.c)
CMSIS_SRC	+= $(wildcard $(CMSIS_DIR)/DeviceSupport/MDR32F9Qx/startup/gcc/*.c)
CMSIS_SRCS	:= $(CMSIS_DIR)/DeviceSupport/MDR32F9Qx/startup/gcc/startup_MDR32F9Qx.s
CMSIS_INC	:= $(CMSIS_DIR)/CoreSupport
CMSIS_INC	+= $(CMSIS_DIR)/DeviceSupport/MDR32F9Qx/startup/gcc
CMSIS_INC	+= $(CMSIS_DIR)/DeviceSupport/MDR32F9Qx/inc
USB_SRC		:= $(wildcard $(USB_DIR)/src/*.c)
USB_INC		:= $(USB_DIR)/inc
OS_DIR		:= FreeRTOS/Source
OS_SRC		:= $(wildcard $(OS_DIR)/*.c)
OS_SRC		+= $(wildcard $(OS_DIR)/portable/GCC/ARM_CM3/*.c)
OS_SRC		+= $(OS_DIR)/portable/MemMang/heap_4.c
OS_INC		:= $(OS_DIR)/include
OS_INC		+= $(OS_DIR)/portable/GCC/ARM_CM3
UCLIBCPP_DIR		:= uClibc++-0.2.4
STL_INC		:= $(UCLIBCPP_DIR)/include
STL_SRC		:= $(wildcard $(UCLIBCPP_DIR)/src/*.cc)
STL_SRC		+= $(wildcard $(UCLIBCPP_DIR)/src/abi/*.cc)

FW_SRC		:= $(wildcard $(SRC_DIR)/*.c)
FW_SRCXX	:= $(wildcard $(SRC_DIR)/*.cc)
HEADERS		:= $(wildcard $(SRC_DIR)/*.h*)
#FW_SRC := src/irhid.c
#FW_SRCXX := 
FW_INC		:= $(SRC_DIR)

SRC_EXCL	:= $(SPL_DIR)/src/MDR32F9Qx_dma.c $(SPL_DIR)/src/MDR32F9Qx_eth.c $(SPL_DIR)/src/MDR32F9Qx_mil_std_1553.c
#comment next line to wipe out libc++ and use gcc for linking.
#SRC_EXCL	+= $(SRC_DIR)/supc++.cc
SRC_EXCL	+= $(SRC_DIR)/spi.cc $(SRC_DIR)/usart.cc $(SRC_DIR)/flash.c

SRC_C := $(SPL_SRC) $(USB_SRC) $(CMSIS_SRC) $(OS_SRC) $(FW_SRC)
SRC_S := $(CMSIS_SRCS)
SRC_CXX := $(FW_SRCXX)
INCLUDE_FOLDERS = $(SPL_INC) $(USB_INC) $(CMSIS_INC) $(OS_INC) $(FW_INC)
SRC_C := $(filter-out $(SRC_EXCL),$(SRC_C))
SRC_CXX := $(filter-out $(SRC_EXCL),$(SRC_CXX))

OBJ := $(SRC_CXX:%.cc=%.o) $(SRC_C:%.c=%.o) $(SRC_S:%.s=%.o)
OBJ := $(OBJ:%=$(BUILD_DIR)/%)  
INCLUDES   := $(INCLUDE_FOLDERS:%=-I%)
DEFINES := $(DEFINE:%=-D%)

CC_FLAGS  = -g -ggdb $(INCLUDES) -I. -O$(OPTIMIZATION) -Wall -std=gnu99 -c -fmessage-length=0 $(MCU_CC) -fdata-sections -ffunction-sections -fno-hosted -fno-builtin -nostdlib -nodefaultlibs -fno-strict-aliasing -save-temps
CXX_FLAGS  = -g -ggdb $(INCLUDES) -I. -O$(OPTIMIZATION) -Wall -std=gnu++14 -c -fmessage-length=0 $(MCU_CC) -fdata-sections -ffunction-sections -fno-builtin -nostdlib -fno-strict-aliasing -fno-rtti -fno-threadsafe-statics -fno-exceptions -save-temps
AS_FLAGS  = $(MCU)
#no semihosting
#LD_SYS_LIBS = -lm -lc -lgcc -lnosys
#LD_FLAGS = -Xlinker --gc-sections $(MCU) --specs=nosys.specs -fPIC $(LD_SYS_LIBS)
#semihosting
LD_SYS_LIBS = -lm -lc -lgcc -lnosys -lrdimon
#LD_FLAGS = -Xlinker --print-map -Xlinker --gc-sections $(MCU) --specs=nosys.specs --specs=rdimon.specs -fPIC $(LD_SYS_LIBS)
LD_FLAGS = -Xlinker --gc-sections $(MCU) --specs=nosys.specs --specs=rdimon.specs -fPIC $(LD_SYS_LIBS)

all: $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/$(TARGET).elf:  $(OBJ)
		@echo LD: $^
		#full RTTI and exception support
		@$(CXX) $(LD_FLAGS) -T $(MCU_NAME).ld -o $@ $^
		#cutoff RTTI and exception support
#		@$(CC) $(LD_FLAGS) -T $(MCU_NAME).ld -o $@ $^
		@echo "\033[31;47m\033[1m"
		@echo '-----------------------------------------------------------'
		@$(SIZE) $(BUILD_DIR)/$(TARGET).elf
		@echo "\033[0m"
		@$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
		@$(OBJCOPY) -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
		@$(OBJDUMP) -h -S -z $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).lss
		@$(NM) -n $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).sym

$(BUILD_DIR)/%.o : %.c $(HEADERS)
		@$(MKDIR) -p `dirname $@`
		@echo "\033[37;44m\033[1m CC:   \033[37;42m$< \033[0m"
		@$(CC) $(CC_FLAGS) $(DEFINES) -c ./$< -o $@

$(BUILD_DIR)/%.o : %.cc $(HEADERS)
		@$(MKDIR) -p `dirname $@`
		@echo "\033[37;44m\033[1mCXX:   \033[37;42m$< \033[0m"
		@echo $(CXX) $(CXX_FLAGS) $(DEFINES) -c ./$< -o $@
		@$(CXX) $(CXX_FLAGS) $(DEFINES) -c ./$< -o $@

$(BUILD_DIR)/%.o : %.s
		@$(MKDIR) -p `dirname $@`
		@echo "\033[37;44m\033[1m AS_C: \033[37;42m$< \033[0m"
		@$(CC) $(INCLUDES) $(DEFINES) -c -x assembler-with-cpp ./$< -o $@

program:
		@st-flash --reset write $(BUILD_DIR)/$(TARGET).bin 0x8000000

clean:
		@$(RM) -rf build/*
		@$(RM) -rf *.i
		@$(RM) -rf *.s

.PHONY : all clean program flash

