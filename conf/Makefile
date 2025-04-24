CC = g++
CFLAGS = -Wall -I../include
LDFLAGS = -lcurl
LDLIBS  = -lsqlite3

SRC_DIR = ../src
BUILD_DIR = ../build
DATA_DIR = ../data
EXPORT_DIR = $(DATA_DIR)/export
JSON_FILE = $(DATA_DIR)/all_measurements.json
BIN = $(BUILD_DIR)/database

SRCS = $(SRC_DIR)/cli.cpp \
       $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/api/ohm_api.cpp \
       $(SRC_DIR)/api/ohm_data.cpp \
       $(SRC_DIR)/config/config_loader.cpp \
       $(SRC_DIR)/inputs/file_source.cpp \
       $(SRC_DIR)/inputs/ohm_source.cpp \
       $(SRC_DIR)/storage/index_manager.cpp \
       $(SRC_DIR)/storage/measurement_handler.cpp \
       $(SRC_DIR)/storage/storage.cpp \
       $(SRC_DIR)/utils/utils.cpp \
       $(SRC_DIR)/benchmark/benchmark.cpp \
       $(SRC_DIR)/benchmark/sqlite_storage.cpp

OBJS = $(SRCS:.cpp=.o)

all: build datadir exportdir jsonfile $(BIN)

build:
	@if [ ! -d $(BUILD_DIR) ]; then echo "Creating $(BUILD_DIR)"; mkdir -p $(BUILD_DIR); fi

datadir:
	@if [ ! -d $(DATA_DIR) ]; then echo "Creating $(DATA_DIR)"; mkdir -p $(DATA_DIR); fi

exportdir:
	@if [ ! -d $(EXPORT_DIR) ]; then echo "Creating $(EXPORT_DIR)"; mkdir -p $(EXPORT_DIR); fi

jsonfile:
	@if [ ! -f $(JSON_FILE) ]; then echo "[]" > $(JSON_FILE); fi

$(BIN): $(SRCS)
	$(CC) $(CFLAGS) -o $(BIN) $(SRCS) $(LDFLAGS) $(LDLIBS)

format:
	find ../src ../include -type f \( -name '*.cpp' -o -name '*.h' \) -exec clang-format -i {} +

clean:
	rm -rf $(BUILD_DIR)

run:
	@$(BIN) || true