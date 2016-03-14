CC=gcc
CFLAGS=-ansi -pedantic -Wall -Wextra -Werror
DEBUG=-g
SRC_DIR=./src
BIN_DIR=./bin

FILES = $(BIN_DIR)/excov

all: clean $(FILES)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS)  $(SRC_DIR)/excov.c -o $(BIN_DIR)/excov $(DEBUG)

.PHONY: clean

clean:
	rm -f $(BIN_DIR)/*
