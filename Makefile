SRC_DIR = src
BUILD_DIR = build

OBJ_SERVER = $(BUILD_DIR)/server.o
SRC_SERVER = $(OBJ_SERVER:$(BUILD_DIR)/%.o=$(SRC_DIR)/%.c)
OBJ_CLIENT = $(BUILD_DIR)/client.o
SRC_CLIENT = $(OBJ_CLIENT:$(BUILD_DIR)/%.o=$(SRC_DIR)/%.c)

CC_SERVER = aarch64-linux-gnu-gcc
CC_CLIENT = gcc

TARGET = server client

all: $(TARGET)

server: $(OBJ_SERVER)
	$(CC_SERVER) -o $@ $<

client: $(OBJ_CLIENT)
	$(CC_CLIENT) -o $@ $<

$(OBJ_SERVER): $(SRC_SERVER)
	mkdir -p $(BUILD_DIR)
	$(CC_SERVER) -c $< -o $@

$(OBJ_CLIENT): $(SRC_CLIENT)
	mkdir -p $(BUILD_DIR)
	$(CC_CLIENT) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
