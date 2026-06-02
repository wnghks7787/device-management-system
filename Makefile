BUILD_DIR = build

OBJ_SERVER = $(BUILD_DIR)/server.o
OBJ_CLIENT = $(BUILD_DIR)/client.o

CC_SERVER = aarch64-linux-gnu-gcc
CC_CLIENT = gcc

LDLIBS = -lpthread

TARGET = server client

all: $(TARGET)

server: $(OBJ_SERVER)
	$(CC_SERVER) -o $@ $< $(LDLIBS)

client: $(OBJ_CLIENT)
	$(CC_CLIENT) -o $@ $<

$(OBJ_SERVER):
	make -C src/server

$(OBJ_CLIENT):
	make -C src/client


clean:
	rm -rf $(TARGET) $(BUILD_DIR)
