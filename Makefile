CC = gcc

SRCS = $(shell find ./src/*.c)

build: $(SRCS)
	$(CC) $^ -o ./spidey-db

.PHONY: build
