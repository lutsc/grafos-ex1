CFLAGS = -Iinclude -lm -lraylib -g -Wall -Wextra

OBJS = build/main.o build/lista_encadeada.o build/matrizes.o build/grafos.o build/menu.o build/busca.o build/fechotransitivo.o build/kosaraju.o build/coloracao.o

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
	./$(TARGET)

# ---------------------------------------------------------------------------
# PERT/CPM: programa independente, sem Raylib e sem nenhuma outra dependência
# ---------------------------------------------------------------------------
PERT_SRCS = src/pert.c src/pert_svg.c src/pert_main.c
PERT_FLAGS = -std=c11 -O2 -Wall -Wextra -Iinclude

.PHONY: all clean run pert pert-static test-pert run-pert

# make pert          -> bin/pert
pert: | bin
	gcc $(PERT_FLAGS) $(PERT_SRCS) -lm -o bin/pert

# make pert-static   -> bin/pert como executável único, que roda em qualquer máquina sem instalar nada
pert-static: | bin
	gcc $(PERT_FLAGS) $(PERT_SRCS) -static -lm -o bin/pert

run-pert: pert
	./bin/pert

test-pert: | bin
	gcc $(PERT_FLAGS) -g tests/testePert.c src/pert.c src/pert_svg.c -lm -o bin/testePert
	./bin/testePert
