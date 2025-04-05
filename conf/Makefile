CC = g++
CFLAGS = -Wall -I../include
LDFLAGS = -lcurl

SRC_DIR = ../src
BUILD_DIR = ../build
DATA_DIR = ../data
EXPORT_DIR = $(DATA_DIR)/export
JSON_FILE = $(DATA_DIR)/all_measurements.json
BIN = $(BUILD_DIR)/database

SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/cli.cpp \
       $(SRC_DIR)/api/ohm_api.cpp \
       $(SRC_DIR)/storage/storage.cpp \
       $(SRC_DIR)/config/config_handler.cpp \
       $(SRC_DIR)/api/ohm_data.cpp \
       $(SRC_DIR)/storage/index_manager.cpp \
       $(SRC_DIR)/utils/utils.cpp \
       $(SRC_DIR)/storage/measurement_handler.cpp \
       $(SRC_DIR)/inputs/ohm_source.cpp \
       $(SRC_DIR)/inputs/file_source.cpp

OBJS = $(SRCS:.cpp=.o)

all: build datadir exportdir jsonfile $(BIN)

build:
	mkdir -p $(BUILD_DIR)

datadir:
	mkdir -p $(DATA_DIR)

exportdir:
	mkdir -p $(EXPORT_DIR)

jsonfile:
	test -f $(JSON_FILE) || echo "[]" > $(JSON_FILE)

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(BIN)
