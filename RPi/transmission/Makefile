#############################################################################
#
# Makefile for librf24 examples on Linux
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu>
# Date:    2013/02/07 (version 1.0)
#
# Description:
# ------------
# use make all and make install to install the examples
#

OS=LINUX
SOC=BCM2835
DRIVER=RPi
CPUFLAGS=-march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard
CFLAGS=-march=armv6zk -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard -Ofast -Wall -pthread 
PREFIX=/usr/local
REMOTE_PREFIX=/usr/local
LIB=rf24
LIBNAME=librf24.so.1.3.0
LIB_VERSION=1.3.0
LIBSYMLINKS=librf24.so.1.3 librf24.so.1 librf24.so
LIBDEPRECATE=librf24-bcm.so
CC=arm-linux-gnueabihf-gcc
#CXX=arm-linux-gnueabihf-g++
LIB_DIR=/usr/local/lib
REMOTE_LIB_DIR=/usr/local/lib
HEADER_DIR=/usr/local/include/RF24
REMOTE_HEADER_DIR=/usr/local/include/RF24
DRIVER_DIR=utility/RPi
ARCH_DIR=utility
SHARED_LINKER_FLAGS= -pthread -shared -Wl,-soname,librf24.so.1
LDCONFIG=ldconfig
REMOTE_LDCONFIG=/sbin/ldconfig
EXAMPLES_DIR=/usr/local/bin
REMOTE_EXAMPLES_DIR=/usr/local/bin

# define all programs
PROGRAMS = js_transmit_imp_o

# *****************************************
BINARY_PREFIX = rf24
SOURCES = $(PROGRAMS:=.cpp)

LIBS=-l$(LIB)
ifeq ($(DRIVER), LittleWire)
	LIBS+= -llittlewire-spi
endif

all: $(PROGRAMS)

$(PROGRAMS): $(SOURCES)
	$(CXX) $(CFLAGS) -I$(HEADER_DIR)/.. -I.. -L$(LIB_DIR) $@.cpp $(LIBS) -o $@

clean:
	@echo "[Cleaning]"
	rm -rf $(PROGRAMS)
