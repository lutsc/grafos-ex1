CFLAGS = -Iinclude -g -Wall -Wextra

OBJS = build/main.o build/lista_encadeada.o build/grafos.o

TARGET = bin/main

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	gcc $(OBJS) -o $(TARGET)

build/%.o: src/%.c | build
	gcc $(CFLAGS) -c $< -o $@

bin build:
	mkdir -p $@

clean:
	rm -rf build bin
