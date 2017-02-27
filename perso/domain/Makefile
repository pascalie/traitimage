ROOT = ../bcl

CFLAGS = -Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS = -L$(ROOT)/lib 
LDLIBS = -lbcl

OBJ = domain.o
BIN = \
	move\
	scroll\
	flip\
	shear

all: $(BIN)

shear: $(OBJ)

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
