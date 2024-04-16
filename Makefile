CC = g++
CFLAGS = -Wall -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Listar todos os arquivos .cpp na pasta src
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Obter nomes dos arquivos sem a extensão .cpp
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Nome do executável
TARGET = $(BIN_DIR)/main

all: $(BUILD_DIR) $(BIN_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilar os arquivos .cpp em objetos na pasta build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Remover todos os arquivos compilados e o executável
clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*

# Rodar o programa
run:
	$(TARGET)
