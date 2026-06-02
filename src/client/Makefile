BUILD_DIR = ../../build

OBJS = client.o
SRCS = $(OBJS:.o=.c)

CC = gcc

$(BUILD_DIR)/$(OBJS): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@
