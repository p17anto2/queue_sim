FILES = src/simulation.c
CC = gcc
BIN_FOLDER = build
BIN = sim

all: run

build_folder:
	ls $(BIN_FOLDER)/ 1>/dev/null 2>/dev/null || mkdir $(BIN_FOLDER)/

debug: build_folder
	$(CC) -g -Wall $(FILES) -o $(BIN_FOLDER)/$(BIN)

release: build_folder
	$(CC) -O3 $(FILES) -o $(BIN_FOLDER)/$(BIN)

run: release
	cd $(BIN_FOLDER); ./$(BIN)
	cd $(BIN_FOLDER); \
		xdg-open averages.png; \
		xdg-open var_arr.png; \
		xdg-open var_dep_average.png; \
		xdg-open var_dep_throughput.png;

clean:
	rm $(BIN_FOLDER)/*
