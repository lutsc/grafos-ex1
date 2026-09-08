CFLAGS = -Iinclude -lm -lraylib -g -Wall -Wextra

OBJS = build/main.o build/lista_encadeada.o build/matrizes.o build/grafos.o

TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	gcc $(OBJS) $(CFLAGS) -o $(TARGET)

build/%.o: src/%.c | build
	gcc $(CFLAGS) -c $< -o $@

bin build:
	mkdir -p $@

clean:
	rm -rf build bin

run:
	make
	./bin/main
