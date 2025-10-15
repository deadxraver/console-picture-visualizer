.PHONY: all clean test debug

HOME=$(shell pwd)
SRC=$(HOME)/src
TEST_SRC=$(HOME)/test
BUILD=$(HOME)/build
TEST_BUILD=$(BUILD)/test
DEBUG_BUILD=$(BUILD)/debug
TARGET=print-pic
TEST_TARGET=run-tests
DEBUG_TARGET=dbg
DEBUG_FLAGS=-g3 -o0
CC=gcc
GDB=gdb
GDB_FLAGS=-ex "layout regs" -ex "break main" -ex "start"

all: build test
	@echo 'built & tested app successfully'
	@echo 'executable can be found in $(BUILD)/$(TARGET)'

build: $(SRC)
	test -e $(BUILD) || mkdir -p $(BUILD)
	$(CC) -o $(BUILD)/$(TARGET) $^/**/*.c $^/*.c

build-test: $(TEST_SRC)
	test -e $(TEST_BUILD) || mkdir -p $(TEST_BUILD)
	$(CC) -o $(TEST_BUILD)/$(TEST_TARGET) $^/*.c $(SRC)/**/*.c

debug: $(SRC)
	mkdir -p $(DEBUG_BUILD)
	$(CC) $(DEBUG_FLAGS) -o $(DEBUG_BUILD)/$(DEBUG_TARGET) $^/*.c $^/**/*.c
	$(GDB) $(GDB_FLAGS) --args $(DEBUG_BUILD)/$(DEBUG_TARGET) -50 $(HOME)/soup.bmp

test: $(SRC) build build-test
	$(TEST_BUILD)/$(TEST_TARGET)

clean:
	rm -rf $(BUILD) $(TEST_BUILD) $(DEBUG_BUILD)
