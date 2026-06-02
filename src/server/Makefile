BUILD_DIR = ../../build

OBJS = server.o
SRCS = $(OBJS:.o=.c)

CC = aarch64-linux-gnu-gcc

$(BUILD_DIR)/$(OBJS): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@
