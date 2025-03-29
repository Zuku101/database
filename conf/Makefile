CC = g++
CFLAGS = -Wall -I../include
LDFLAGS = -lcurl

SRC_DIR = ../src
BUILD_DIR = ../build
DATA_DIR = ../data
JSON_FILE = $(DATA_DIR)/all_measurements.json
BIN = $(BUILD_DIR)/database

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/cli.cpp \
       $(SRC_DIR)/api/ohm_api.cpp \
       $(SRC_DIR)/storage/storage.cpp \
	   $(SRC_DIR)/config/config_handler.cpp \
	   $(SRC_DIR)/ohm_data.cpp

OBJS = $(SRCS:.cpp=.o)

all: build datadir jsonfile $(BIN)

build:
	mkdir -p $(BUILD_DIR)

datadir:
	mkdir -p $(DATA_DIR)

jsonfile:
	test -f $(JSON_FILE) || echo "[]" > $(JSON_FILE)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(BIN)
