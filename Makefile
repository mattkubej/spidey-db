CC = gcc
BUILD_DIR = build

SRCS = $(shell find ./src/*.c)

build: $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $^ -o ./build/spidey-db

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: build clean
