mkdir -p build
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/main.c -o build/main.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/lista_encadeada.c -o build/lista_encadeada.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/matrizes.c -o build/matrizes.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/grafos.c -o build/grafos.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/menu.c -o build/menu.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/busca.c -o build/busca.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/fechotransitivo.c -o build/fechotransitivo.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/kosaraju.c -o build/kosaraju.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/coloracao.c -o build/coloracao.o
gcc -Iinclude -lm -lraylib -g -Wall -Wextra -c src/dijkstra.c -o build/dijkstra.o
mkdir -p bin
gcc build/main.o build/lista_encadeada.o build/matrizes.o build/grafos.o build/menu.o build/busca.o build/fechotransitivo.o build/kosaraju.o build/coloracao.o build/dijkstra.o -Iinclude -lm -lraylib -g -Wall -Wextra -o bin/main

