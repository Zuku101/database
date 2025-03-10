CC = g++
CFLAGS = -Wall -I../include
LDFLAGS = -lcurl
SRC_DIR = ../src
BUILD_DIR = ../build
BIN = $(BUILD_DIR)/database

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/cli.cpp $(SRC_DIR)/api/ohm_api.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(BIN)

$(BIN): $(SRCS)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(BIN)
