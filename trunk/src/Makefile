#
#  Makefile for yasa/src
#

# compiler and linker settings
CC=cc
CFLAGS=-O -pipe -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -pedantic -std=c99
LDFLAGS=

# program and its parts
SRCS = yasa.c
OBJS = stg.o yasa.o
LIBS =


.PHONY: clean distclean

# user targets
all: yasa

run: all
	./yasa

clean:
	-rm -f yasa $(OBJS)

distclean: clean
	-rm -f *~

# object dependencies
yasa: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

# header dependencies
stg.c: stg.h
