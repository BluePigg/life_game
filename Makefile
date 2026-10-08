CC = gcc
TARGET = lifegame

all: $(TARGET)

$(TARGET): lifegame.c
	$(CC) -o $(TARGET) lifegame.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
