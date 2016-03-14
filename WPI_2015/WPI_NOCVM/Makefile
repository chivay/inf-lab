CC=gcc
CFLAGS=-ansi -pedantic -Wall -Wextra -Werror
DEBUG=-g
SRC_DIR=./src
BIN_DIR=./bin

FILES = $(BIN_DIR)/nocvm

all: clean $(FILES)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS)  $(SRC_DIR)/nocvm.c -o $(BIN_DIR)/nocvm $(DEBUG)

.PHONY: clean

clean:
	rm -f $(BIN_DIR)/*
