ROOT = ../bcl

CFLAGS = -Wall -Wextra -Werror -std=c99 -I$(ROOT)/include 
LDFLAGS = -L$(ROOT)/lib 
LDLIBS = -lbcl -lm

OBJ = domain.o
BIN = colorization\
		test\


all: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN) *.ppm

.PHONY: all clean 
