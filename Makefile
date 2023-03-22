CC=gcc

prog: sudoku.c
	$(CC) sudoku.c -o sudoku

run: sudoku
	./prog arg1 arg2
