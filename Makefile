CFLAGS = -Wall -std=c++11 -Iinclude -lGL -lglfw -lGLEW
CC = g++

SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SOURCES))

INC_DIR = include
HEADERS = $(wildcard $(INC_DIR)/*.h)

OUT_DIR = bin

.PHONY: all clean

all: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(OUT_DIR)/gl_draw

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f bin/* src/*.o
