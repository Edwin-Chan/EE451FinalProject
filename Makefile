CC = g++
DEBUG = -g -Wall
C11 = -std=c++11
FAST = -O3

all: lzw_serial_encode

lzw_serial_encode: lzw_serial_encode.cpp
	$(CC) $(DEBUG) $(C11) $< -o $@

.PHONY: clean

clean: 
	rm -f lzw_serial_encode