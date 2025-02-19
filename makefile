CC=gcc
CFLAGS=-I

main: main.c png.h
	$(CC) -o main main.c
