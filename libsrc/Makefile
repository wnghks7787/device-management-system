LIB_DIR = ../lib

OBJS = led.o
LIBS = $(OBJS:%.o=$(LIB_DIR)/lib%.so)

CC = aarch64-linux-gnu-gcc
CFLAG = -c -ePIC
CFLAG_LIB = -lwiringPi -lcrypt

$(LIBS): $(OBJS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $< $(CFLAG_LIB)

%.o: %.c
	$(CC) $(CFLAG) $<
