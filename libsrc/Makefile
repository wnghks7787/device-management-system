LIB_DIR = ../lib
BUILD_DIR = ../build

SRCS = led.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
LIBS = $(SRCS:%.c=$(LIB_DIR)/lib%.so)

CC = aarch64-linux-gnu-gcc
CFLAG = -c -ePIC
CFLAG_LIB = -lpthread -lwiringPi -lcrypt

$(LIBS): $(OBJS)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $< $(CFLAG_LIB)


$(OBJS): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAG) $< -o $@
