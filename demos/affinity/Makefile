ifeq ($(COMP),)
COMP=gnu
endif

OPT=-O0
LDFLAGS=-lm

ifeq ($(COMP),gnu)
CC=mpicc
CFLAGS = $(OPT) -fopenmp
endif

ifeq ($(COMP),intel)
CC=mpicc
CFLAGS = $(OPT) -qopenmp
endif

ifeq ($(COMP),clang)
CC=mpicc
CFLAGS = $(OPT) -fopenmp
endif

all: affinity

affinity: affinity_test.c
	$(CC) -o $(CFLAGS) -o $@  $< $(LDFLAGS)

clean:
	rm -f *.o affinity
