tetris-exec: main.c
	gcc -O2 -Wall -pedantic -std=c99 -lncurses -lm main.c -o tetris-exec
