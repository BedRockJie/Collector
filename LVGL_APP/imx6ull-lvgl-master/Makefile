#
# Makefile
#
# https://www.100ask.net/
#
CC := riscv64-unknown-linux-gnu-gcc
#CC ?= gcc
LVGL_DIR ?= ${shell pwd}
LVGL_DIR_NAME ?= lvgl

#WARNINGS = -Wall -Wextra \
           -Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wpointer-arith -Wuninitialized \
           -Wunreachable-code -Wreturn-type -Wmultichar -Wdouble-promotion -Wclobbered -Wdeprecated  \
           -Wempty-body -Wshift-negative-value \
           -Wtype-limits -Wsizeof-pointer-memaccess
#WARNINGS = -Werror -Wextra \
           -Wshadow -Wundef -Wmaybe-uninitialized -Wmissing-prototypes -Wpointer-arith -Wuninitialized \
           -Wunreachable-code -Wreturn-type -Wmultichar -Wdouble-promotion -Wclobbered -Wdeprecated  \
           -Wempty-body -Wshift-negative-value -Wstack-usage=2048 \
           -Wtype-limits -Wsizeof-pointer-memaccess

#-Wno-unused-value -Wno-unused-parameter
OPTIMIZATION ?= -O3 -g0 -Wall -Wshadow -Wundef -Wmaybe-uninitialized 

LIB_LINK_NAPI = /home/gaojies/workspace/d1-tina-open/out/
WORKSPACE = /home/gaojies/workspace/d1-tina-open/

CFLAGS ?= -I$(LVGL_DIR)/ $(DEFINES) $(WARNINGS) $(OPTIMIZATION) -I$(LVGL_DIR) -I. -I $(LIB_LINK_NAPI)d1-nezha/compile_dir/target/libuapi/src/include

LDFLAGS ?=  -L$(LIB_LINK_NAPI)d1-nezha/staging_dir/target/usr/lib -L$(LIB_LINK_NAPI)d1-nezha/staging_dir/target/lib -L$(WORKSPACE)prebuilt/gcc/linux-x86/riscv/toolchain-thead-glibc/riscv64-glibc-gcc-thead_20200702/usr/lib -L$(WORKSPACE)prebuilt/gcc/linux-x86/riscv/toolchain-thead-glibc/riscv64-glibc-gcc-thead_20200702/lib -znow -zrelro -luapi -lm -lpthread

#LDFLAGS ?=  -lm -linput -lpng
BIN ?= output/card22222


#Collect the files to compile
MAINSRC = ./main.c


#CSRCS += lv_drivers/display/fbdev.c


include ./lvgl/lvgl.mk
include ./lv_drivers/lv_drivers.mk
include ./lv_100ask/lv_100ask.mk
include ./lv_lib_png/lv_lib_png.mk
#include ./lv_examples/lv_examples.mk

OBJEXT ?= .o

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

## MAINOBJ -> OBJFILES

all: default

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC $<"

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	$(CC) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS) 

clean:
	rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ)


