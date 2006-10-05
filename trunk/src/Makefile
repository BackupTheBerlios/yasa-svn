#
#  Makefile for yasa/src
#

# compiler and linker settings
CC=cc
CFLAGS=-O -pipe -Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual -pedantic -std=c99
#DEBUG=-DNDEBUG  # if NDEBUG is defined: leave out assert() macros
#LDFLAGS=-lcrypt
LDFLAGS=

# program and its parts
SRCS = yasa.c
OBJS = seqsa.o ssf.o stg.o tsort.o worstcase.o yasa.o
LIBS = -lm


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
	$(CC) $(CFLAGS) $(DEBUG) -c $<

# header dependencies
ssf.c: ssf.h stg.h
stg.c: stg.h
tsort.c: stg.h tsort.h
worstcase.c: ssf.h stg.h tsort.h worstcase.h
yasa.c: seqsa.h worstcase.h
