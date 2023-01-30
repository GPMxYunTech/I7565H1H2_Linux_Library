#PJ = i7565H1H2
SHELL = /bin/sh

Bit := $(shell getconf LONG_BIT)

ifeq ($(LinPAC), ARM)
        CROSS_COMPILE   = arm-linux-gnueabihf-
        DLIBS = ../lib/libI7565H1H2_arm.so.1.0 ../lib/libi7k_arm.so.1.0
        SLIBS = ../lib/libI7565H1H2_arm.a -L ../lib -li7k_arm
        PJ = i7565H1H2_arm
else
        CROSS_COMPILE   =

ifeq ($(Bit), 64)
        DLIBS = ../lib/libI7565H1H2_64.so.1.0 ../lib/libi7k_64.so.1.0
        SLIBS = ../lib/libI7565H1H2_64.a -L ../lib -li7k_64
else
        DLIBS = ../lib/libI7565H1H2.so.1.0 ../lib/libi7k.so.1.0
        SLIBS = ../lib/libI7565H1H2.a -L ../lib -li7k
endif
        PJ = i7565H1H2
endif

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump

MACRO = -DI7565H1H2
CFLAGS =  $(MACRO) -I. -I../include
LDFLAGS = -lgcc_s -lc -no-pie
AM_LDFLAGS = -pthread

all: i7565H1H2_a i7565H1H2_so

i7565H1H2_a: i7565H1H2.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ i7565H1H2.o $(SLIBS) $(AM_LDFLAGS)
i7565H1H2_so: i7565H1H2.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ i7565H1H2.o $(DLIBS) $(AM_LDFLAGS)
	rm -f i7565H1H2.o

indent:
	indent -kr -ts4 *.c

clean:
	rm -f *.o *~ *.tmp *.bak *_a *_so \#* .\#*

