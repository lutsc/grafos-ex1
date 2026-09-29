CFLAGS = -Iinclude -lm -lraylib -g -Wall -Wextra

OBJS = ../build/lista_encadeada.o ../build/matrizes.o ../build/grafos.o ../build/busca.o ../build/fechotransitivo.o ../build/kosaraju.o ../build/coloracao.o ../build/dijkstra.o


%:%.c
	gcc -g -I../include -lraylib $(OBJS) $^ -o bin/$@
