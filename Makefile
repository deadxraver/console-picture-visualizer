.PHONY: all clean test

SRC=src
BUILD=build
TARGET=print-pic
CC=gcc

all: build test
	@echo 'built & tested app successfully'
	@echo 'executable can be found in $(BUILD)/$(TARGET)'

build: $(SRC)
	test -e $(BUILD) || mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(TARGET) $^/**

test: $(SRC) build
	@echo 'TODO'

clean:
	rm -rf $(BUILD)
