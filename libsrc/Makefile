LIB_DIR = ../lib
BUILD_DIR = ../build

SRCS = led.c buzzer.c cds.c fnd.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
LIBS = $(SRCS:%.c=$(LIB_DIR)/lib%.so)

CC = aarch64-linux-gnu-gcc
CFLAG = -c -fPIC
CFLAG_LIB = -lpthread -lwiringPi -lcrypt

all: $(LIBS)

$(LIB_DIR)/lib%.so: $(BUILD_DIR)/%.o
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $< $(CFLAG_LIB)


$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAG) $< -o $@
