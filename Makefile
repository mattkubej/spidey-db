CC = gcc
CFLAGS = -Wall -Wextra
BUILD_DIR = build

SRCS = $(shell find ./src/*.c)

build: $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -g -o ./build/spidey-db

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: build clean
