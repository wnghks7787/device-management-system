BUILD_DIR = build
BIN_DIR = bin

OBJ_SERVER = $(BUILD_DIR)/server.o
OBJ_CLIENT = $(BUILD_DIR)/client.o

CC_SERVER = aarch64-linux-gnu-gcc
CC_CLIENT = gcc

LDLIBS = -lpthread

TARGET = $(BIN_DIR)/server $(BIN_DIR)/client

all: $(TARGET)

$(BIN_DIR)/server: $(OBJ_SERVER)
	mkdir -p $(BIN_DIR)
	$(CC_SERVER) -o $@ $< $(LDLIBS) -ldl

$(BIN_DIR)/client: $(OBJ_CLIENT)
	mkdir -p $(BIN_DIR)
	$(CC_CLIENT) -o $@ $<

$(OBJ_SERVER):
	make -C src/server

$(OBJ_CLIENT):
	make -C src/client

send:
	tar -cvzf server.tar.gz bin/ lib/
	~/scp.sh server.tar.gz


clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR) server.tar.gz
