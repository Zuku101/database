CC = g++
CFLAGS = -Wall -I../include
LDFLAGS = -lcurl
SRC_DIR = ../src
BUILD_DIR = ../build
OUTPUT_DIR = ../output
BIN = $(BUILD_DIR)/database

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/cli.cpp $(SRC_DIR)/api/ohm_api.cpp
OBJS = $(SRCS:.cpp=.o)

all: build output $(BIN)

build:
	mkdir -p $(BUILD_DIR)

output:
	mkdir -p $(OUTPUT_DIR)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(OUTPUT_DIR)

run: all
	$(BIN)
