#
# Samantha Foley
# CS 441/541: Project 4
#
#####################################################################
#
# Type "make" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=gcc
CFLAGS=-Wall -g -O0 -I../lib
LDFLAGS=-pthread

PROGS=diners-v1 diners-v2

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
all: $(PROGS)

#
# Dining Philosophers V1
#
diners-v1: diners-v1.c diners.h ../lib/semaphore_support.o
	$(CC) -o diners-v1 diners-v1.c ../lib/semaphore_support.o $(CFLAGS) $(LDFLAGS)

#
# Dining Philosophers V2
#
diners-v2: diners-v2.c diners.h ../lib/semaphore_support.o
	$(CC) -o diners-v2 diners-v2.c ../lib/semaphore_support.o $(CFLAGS) $(LDFLAGS)

#
# Semaphore support library
#
../lib/semaphore_support.o: ../lib/semaphore_support.h ../lib/semaphore_support.c
	cd ../lib && make lib

#
# Cleanup the files that we have created
#
clean:
	$(RM) $(PROGS) *.o
	$(RM) -rf *.dSYM
	cd ../lib && make clean
