#######
# Makefile for peep compiler
#######

CC = gcc
CFLAGS = -O0 -g -Wall -Werror

all: peepcc 

peepcc: hmlsim.o peepcc.o infToPstf.o 
	$(CC) $(CFLAGS) -o peepcc peepcc.o infToPstf.o hmlsim.o

peepcc.o : peepcc.c peepcc.h infToPstf.h
	$(CC) $(CFLAGS) -c peepcc.c

infToPstf.o : infToPstf.c infToPstf.h peepcc.h
	$(CC) $(CFLAGS) -c infToPstf.c

hmlsim.o : hmlsim.c peepcc.h
	$(CC) $(CFLAGS) -c hmlsim.c


clean:
	rm -f *.o *~peepcc
	rm -rf *.dSYM
	(cd traces; rm -f *~)
