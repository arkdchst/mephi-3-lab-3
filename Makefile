.PHONY: all clean
.DEFAULT_GOAL := all

INCLUDE_DIR := include
SRC_DIR := src
BIN_DIR := bin

G++ := g++ -g -std=c++17 -I$(INCLUDE_DIR)

all: bin/test bin/ui

bin/test: $(SRC_DIR)/test.cpp $(INCLUDE_DIR)/*
	mkdir -p $(BIN_DIR)
	
	$(G++) -lgtest -lgtest_main $(SRC_DIR)/test.cpp -o $(BIN_DIR)/test

bin/ui: $(SRC_DIR)/ui.cpp $(INCLUDE_DIR)/*
	mkdir -p $(BIN_DIR)
	
	$(G++) $(SRC_DIR)/ui.cpp -o $(BIN_DIR)/ui


clean:
	rm -rf bin/test bin/ui