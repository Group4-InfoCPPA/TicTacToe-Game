CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lws2_32

SRC_DIR = ./src
BIN_DIR = ./bin

TARGET = $(BIN_DIR)/testServer.exe

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(BIN_DIR)\*.o $(TARGET)

run: all
	$(TARGET)
