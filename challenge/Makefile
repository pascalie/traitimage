ROOT=../bcl

CC = gcc
CFLAGS  = -Wall -Wextra -std=c99 -I$(ROOT)/include 
LDFLAGS = -L$(ROOT)/lib -lbcl -lm

PEPPER=../data/pepper.ppm

OBJ= threshold.o\
	prog2.o\
	findGroupes.o\
	pipeline.o

SRC= threshold.c\
	prog2.c\
	findGroupes.c\
	pipeline.c

BIN= threshold\
	prog2\
	findGroupes\
	pipeline

all: $(BIN)

threshold: threshold.o threshold.o
	$(CC) threshold.o $(LDFLAGS) -o $@

threshold.o: threshold.c

prog2: prog2.o prog2.o
	$(CC) prog2.o $(LDFLAGS) -o $@

prog2.o: prog2.c 

findGroupes: findGroupes.o findGroupes.o
	$(CC) findGroupes.o $(LDFLAGS) -o $@

findGroupes.o: findGroupes.c 

pipeline: pipeline.o pipeline.o
	$(CC) pipeline.o $(LDFLAGS) -o $@

pipeline.o: pipeline.c 

clean:
	rm -f $(OBJ) *~ 

cleanall: clean
	rm -rf $(BIN) *.ppm* 

.PHONY: all clean cleanall 
