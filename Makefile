#######
# Makefile for peep compiler
#######

CC = gcc
CFLAGS = -O0 -g -Wall -Werror

all: peepcc test-i2p

peepcc: peepcc.o infToPstf.o
	$(CC) $(CFLAGS) -o peepcc peepcc.o infToPstf.o

peepcc.o : peepcc.c peepcc.h infToPstf.h
	$(CC) $(CFLAGS) -c peepcc.c

infToPstf.o : infToPstf.c infToPstf.h peepcc.h
	$(CC) $(CFLAGS) -c infToPstf.c

test-i2p: testi2p.o infToPstf.o peepcc.o
	$(CC) $(CFLAGS) -o testi2p testi2p.o infToPstf.o peepcc.o

testi2p.o : testi2p.c peepcc.h infToPstf.h
	$(CC) $(CFLAGS) -c testi2p.c

clean:
	rm -f *.o *~peepcc
	rm -rf *.dSYM
	(cd traces; rm -f *~)
