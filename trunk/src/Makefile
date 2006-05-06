#
#  Makefile for yasa/src
#


CC=cc
CFLAGS=-O -pipe -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -pedantic -std=c99
LDFLAGS=

SRCS = yasa.c
OBJS = yasa.o
LIBS =


.PHONY: clean distclean


all: yasa

run: all
	./yasa


clean:
	-rm -f yasa $(OBJS)

distclean: clean
	-rm -f *~


yasa: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<
