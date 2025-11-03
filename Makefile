CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGET=wish
SOURCE=wish.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

.PHONY: all clean

