ROOT=../bcl

CC = gcc
CFLAGS  = -Wall -Wextra -std=c99 -I$(ROOT)/include 
LDFLAGS = -L$(ROOT)/lib -lbcl -lm

PEPPER=../data/pepper.ppm

OBJ= domain.o\
	move.o\
	scroll.o\
	flip.o\
	shear.o\
	rotate.o\
	wave.o\
	sphere.o

SRC= domain.h\
	domain.c\
	move.c\
	scroll.c\
	flip.c\
	shear.c\
	rotate.c\
	wave.c\
	sphere.c


BIN=move\
	scroll\
	flip\
	shear\
	rotate\
	wave\
	sphere


all: $(BIN)

move: move.o move.o
	$(CC) move.o $(LDFLAGS) -o $@
	
scroll: scroll.o
	$(CC) scroll.o $(LDFLAGS) -o $@
	
flip: flip.o
	$(CC) flip.o $(LDFLAGS) -o $@
	
shear: shear.o domain.o
	$(CC) shear.o domain.o $(LDFLAGS) -o $@

rotate: rotate.o domain.o
	$(CC) rotate.o domain.o $(LDFLAGS) -o $@

wave: wave.o domain.o
	$(CC) wave.o domain.o $(LDFLAGS) -o $@

sphere: sphere.o domain.o
	$(CC) sphere.o domain.o $(LDFLAGS) -o $@

move.o: move.c 
shear.o: shear.c domain.h domain.o
domain.o: domain.h domain.c
flip.o: flip.c
scroll.o: scroll.c
rotate.o: rotate.c domain.h domain.o
wave.o: wave.c domain.h domain.o

test: test-move test-shear

test-move: all
	./move 0 30 $(PEPPER) a.ppm; pvisu a.ppm; read 
	./move 30 0 $(PEPPER) a.ppm; pvisu a.ppm; read
	./move 30 30 $(PEPPER) a.ppm; pvisu a.ppm; read
	./move -30 -30 $(PEPPER) a.ppm; pvisu a.ppm; read 	
	rm -rf a.ppm;
	killall pvisu

test-shear: all
	./shear h 30  $(PEPPER) a.ppm; pvisu a.ppm; read 
	./shear h -30  $(PEPPER) a.ppm; pvisu a.ppm; read
	./shear v 30 $(PEPPER) a.ppm; pvisu a.ppm; read
	./shear v -30 $(PEPPER) a.ppm; pvisu a.ppm; read
	rm -rf a.ppm;
	killall pvisu

clean:
	rm -f $(OBJ) *~ 

nettoie:
	rm -f *.png *.gif

cleanall: clean nettoie
	rm -rf $(BIN) *.ppm* 

.PHONY: all clean cleanall 
