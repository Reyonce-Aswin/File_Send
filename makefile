CC=gcc
CFLAGS=-Wall -Iinclude
LIBS=-lncursesw -lpthread

SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/obj/%.o)

TARGET=build/filesend

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

build/obj/%.o: src/%.c
	mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build

install:
	cp $(TARGET) /usr/local/bin/

uninstall:
	rm /usr/local/bin/filesend