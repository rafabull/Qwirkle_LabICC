OBJS	= main.o functions.o
SOURCE	= main.c functions.c
HEADER	= functions.h
OUT	= a.exe
CC	 = gcc
FLAGS	 = -g -c
LFLAGS	 = 

all: main.o functions.o
	gcc -g main.o functions.o   

main.o: main.c
	gcc -g -c main.c 

functions.o: functions.c
	gcc -g -c functions.c 

clean:
	del main.o functions.o qwirckle.exe a.exe

run: a.exe
	./a.exe