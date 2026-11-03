CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGET = bank

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe accounts.dat transactions.txt

.PHONY: all clean
