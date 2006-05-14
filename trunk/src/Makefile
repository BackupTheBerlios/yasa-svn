#
#  Makefile for yasa/src
#

# compiler and linker settings
CC=cc
CFLAGS=-O -pipe -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -pedantic -std=c99
LDFLAGS=

# program and its parts
SRCS = yasa.c
OBJS = ssf.o stg.o worstcase.o yasa.o
LIBS =


.PHONY: clean distclean

# user targets
all: yasa

run: all
	./yasa

clean:
	-rm -f yasa $(OBJS)

distclean: clean
	-rm -f *~ *.core

# object dependencies
yasa: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

# header dependencies
ssf.c: ssf.h stg.h
stg.c: stg.h
worstcase.c: ssf.h stg.h worstcase.h
yasa.c: worstcase.h
