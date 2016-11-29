# the compiler and compiler flags:
CC = gcc
CFLAGS = -O3 -Wall -Werror -g -std=c99
LFLAGS = -lm

SRC=$(wildcard *.c)

TARGET=tsp_2opt

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	$(RM) $(TARGET) 