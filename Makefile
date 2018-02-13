CC=g++

CFLAGS= -std=c++11 

all: minesweeper

minesweeper:
	$(CC) minesweeper.cpp -o minesweeper $(CFLAGS)

clean:
	rm minesweeper

