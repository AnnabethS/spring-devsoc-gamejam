CC = gcc

TARGET = game
SOURCES = $(wildcard src/*.c)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm

$(TARGET): $(SOURCES)
	$(CC) $^ -O3 -o $@ $(LINKER_FLAGS)

.PHONY: clean

clean:
	rm game
