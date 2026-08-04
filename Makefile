CFLAGS += -I./include

OBJS = lista_encadeada.o

all:
	gcc -c src/lista_encadeada.c -o build/lista_encadeada.o
	gcc $(CFLAGS) src/main.c build/$(OBJS) -o bin/main

commands:
	compiledb make all

clean:
	rm -rf build/*
	rm -rf bin/*

