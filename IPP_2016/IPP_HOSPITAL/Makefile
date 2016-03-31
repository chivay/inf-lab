BUILD_DIR = build
BIN_DIR = bin
SRC_DIR = src
SRC_EXT = c
INC = -I include

CC = gcc
TARGET = hospital
CFLAGS = -Wall

all: executable

debug: CFLAGS +=  -g
debug: TARGET = hospital.dbg
debug: executable

executable: $(BUILD_DIR)/hospital.o $(BUILD_DIR)/list.o $(BUILD_DIR)/parse.o $(BUILD_DIR)/structure.o
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$(TARGET) $(CFLAGS) $^

$(BUILD_DIR)/list.o: $(SRC_DIR)/list.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

$(BUILD_DIR)/parse.o: $(SRC_DIR)/parse.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

$(BUILD_DIR)/structure.o: $(SRC_DIR)/structure.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

$(BUILD_DIR)/hospital.o: $(SRC_DIR)/hospital.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean:
	$(RM) -r $(BUILD_DIR) $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TARGET).dbg

.PHONY: clean