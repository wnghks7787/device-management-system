REMOTE_USER = wnghks7787
REMOTE_HOST = 100.69.64.62
REMOTE_DIR = /home/wnghks7787/server
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

OBJ_SERVER = $(BUILD_DIR)/server.o
OBJ_CLIENT = $(BUILD_DIR)/client.o

LIBS = $(LIB_DIR)/led.so

CC_SERVER = aarch64-linux-gnu-gcc
CC_CLIENT = gcc

LDLIBS = -lpthread -lwiringPi -lcrypt

TARGET = $(BIN_DIR)/server $(BIN_DIR)/client

all: $(TARGET)

$(BIN_DIR)/server: $(LIBS) $(OBJ_SERVER)
	mkdir -p $(BIN_DIR)
	$(CC_SERVER) -o $@ $(OBJ_SERVER) $(LDLIBS) -ldl

$(BIN_DIR)/client: $(OBJ_CLIENT)
	mkdir -p $(BIN_DIR)
	$(CC_CLIENT) -o $@ $<

$(LIBS):
	make -C libsrc

$(OBJ_SERVER):
	make -C src/server

$(OBJ_CLIENT):
	make -C src/client

deploy:
	ssh $(REMOTE_USER)@$(REMOTE_HOST) 'mkdir -p $(REMOTE_DIR)'
	ssh $(REMOTE_USER)@$(REMOTE_HOST) 'mkdir -p $(REMOTE_DIR)/$(BIN_DIR)'
	scp $(BIN_DIR)/server $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)/$(BIN_DIR)
	scp -r $(LIB_DIR) $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_DIR)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR) $(LIB_DIR)
