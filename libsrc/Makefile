LIB_DIR = ../lib

OBJS = led.o
LIBS = $(OBJS:%.o=$(LIB_DIR)/lib%.so)

CC = aarch64-linux-gnu-gcc
CFLAG = -c -ePIC
CFLLAG_LIB = -lwiringPi -lcrypt

$(LIBS): $(OBJS)
	$(CC) -shared -o $@ $(OBJ_LIB)$(CFLAG_LIB)

%.o: %.c
	$(CC) $(CFLAG) $<
