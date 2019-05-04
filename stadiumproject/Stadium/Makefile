CC=gcc
CFLAGS=-I. -pthread -std=c99
DEPS = stadium.h
OBJ = stadium.o main.o 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)