# Compiler & flags
CC = gcc
CFLAGS = -Wall -Wextra `pkg-config --cflags dbus-1` -Iinc
LDFLAGS  := $(shell pkg-config --libs dbus-1) -lcrypto 

# Directories
SRC_DIR := src
BUILD_DIR := build

# Automatically find all .c source files in SRC_DIR
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

# Target binary
TARGET := main

# Default rule
all: $(TARGET)

# Link target
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile each .c to .o (creating subdirectories under build)
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
