CC=gcc
CFLAGS=-ansi -pedantic -Wall -Wextra
DEBUG=-g
SRC_DIR=./src
BIN_DIR=./bin

FILES = $(BIN_DIR)/boulder

all: clean $(FILES)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS)  $(SRC_DIR)/boulder.c -o $(BIN_DIR)/boulder $(DEBUG)

.PHONY: clean

clean:
	rm -f $(BIN_DIR)/*
