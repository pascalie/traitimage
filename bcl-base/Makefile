ROOT=../bcl

CFLAGS= -Wall -Wextra -Werror -std=c99 -I$(ROOT)/include
LDFLAGS= -L$(ROOT)/lib
LDLIBS= -lbcl
BIN=\
	test1\
	extract-subimage\
	extract-channel\
	color2mean

all: $(BIN)

clean:
	$(RM) $(BIN) *.ppm

.PHONY:
	clean all
