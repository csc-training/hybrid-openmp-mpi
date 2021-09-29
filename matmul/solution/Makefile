BIN_DIR=.
PROGRAM=$(BIN_DIR)/matmul

MKL_HOME=$(MKLROOT)

CC=gcc
CFLAGS=-O3 -std=gnu99 -fopenmp
LDFLAGS=-fopenmp
#VERBOSE=-DVERBOSE

all: $(PROGRAM) 

$(PROGRAM): matmul.c 
	$(CC) $(CFLAGS) $(VERBOSE) -o $@ $^ $(LDFLAGS)

$(BIN_DIR): 
	mkdir $@

clean:
	rm -rf $(PROGRAM) *.o

wipe: clean
	rm -rf *.out *.err
