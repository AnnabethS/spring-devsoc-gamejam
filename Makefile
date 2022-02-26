CC = gcc

TARGET = game
SOURCES = $(wildcard src/*.c)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lm

$(TARGET): $(SOURCES)
	$(CC) $^ -O2 -o $@ $(LINKER_FLAGS)
