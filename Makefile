.PHONY: all clean test

HOME=$(shell pwd)
SRC=$(HOME)/src
TEST_SRC=$(HOME)/test
BUILD=$(HOME)/build
TEST_BUILD=$(BUILD)/test
TARGET=print-pic
TEST_TARGET=run-tests
CC=gcc

all: build test
	@echo 'built & tested app successfully'
	@echo 'executable can be found in $(BUILD)/$(TARGET)'

build: $(SRC)
	test -e $(BUILD) || mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(TARGET) $^/**/*.c $^/*.c

build-test: $(TEST_SRC)
	test -e $(TEST_BUILD) || mkdir -p $(TEST_BUILD)
	$(CC) -o $(TEST_BUILD)/$(TEST_TARGET) $^/*.c $(SRC)/**/*.c

test: $(SRC) build build-test
	$(TEST_BUILD)/$(TEST_TARGET)

clean:
	rm -rf $(BUILD) $(TEST_BUILD)
