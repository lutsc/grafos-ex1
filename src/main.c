#include <stdio.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"

int main(void) {
	struct Graph graph;
	bool ** matriz = NULL;
	size_t i, j;

	// 1. Direcionado ou não
	puts("\nO grafo é dirigido?\n[1] Sim\n[0] Não");
	if (scanf("%zu", &graph.directed) != 1 || (graph.directed > 1)) {
		printf("Opção inválida.\n");
		return 1;
	}

	// 2. Quantidade de vértices
	printf("\nQuantos vértices terão em seu grafo (máx. 26): ");
	if (scanf("%zu", &graph.verticesQtd) != 1 || graph.verticesQtd == 0 || graph.verticesQtd > 26) {
		printf("Opção inválida.\n");
		return 1;
	}

	printf("Dirigido: %zu, Quantidade: %zu\n", graph.directed, graph.verticesQtd);

	// Vetor de listas (lista de adjacência), uma struct List por vértice
	if (iniciarGrafo(&graph, (int)graph.verticesQtd) != 0)
		return 1;

	// Matriz de adjacência
	if (gerarMatrizAdjacente(&matriz, (uint32_t)graph.verticesQtd) != 0) {
		printf("Erro ao gerar matriz de adjacência.\n");
		free(graph.array);
		return 1;
	}

	// 3. Arestas/arcos
	size_t qtdArestas;
	printf("\nOs vértices são identificados pelas letras A, B, C, ...\n");
	printf("Quantas arestas/arcos terá o grafo: ");
	if (scanf("%zu", &qtdArestas) != 1) {
		printf("Opção inválida.\n");
		return 1;
	}

	for (i = 0; i < qtdArestas; i++) {
		size_t origem, destino;

		printf("\nAresta %zu:\n", i + 1);
		printf("Vértice de origem (0 a %zu): ", graph.verticesQtd - 1);
		scanf("%zu", &origem);
		printf("Vértice de destino (0 a %zu): ", graph.verticesQtd - 1);
		scanf("%zu", &destino);

		if (origem >= graph.verticesQtd || destino >= graph.verticesQtd) {
			printf("Vértice inválido, aresta ignorada.\n");
			continue;
		}

		// Insere na matriz de adjacência
		if (inserirNaMatrizAdjacente(matriz, (uint32_t)graph.verticesQtd, (uint32_t)origem, (uint32_t)destino) == 1)
			printf("Aviso: aresta já existia na matriz.\n");

		// Insere na lista de adjacência (rótulo do destino como letra)
		char * rotulo = malloc(sizeof(char));
		*rotulo = (char)('A' + destino);
		insereListaFim(&graph.array[origem], rotulo);

		// Se não for dirigido, insere também o caminho inverso
		if (graph.directed == 0) {
			inserirNaMatrizAdjacente(matriz, (uint32_t)graph.verticesQtd, (uint32_t)destino, (uint32_t)origem);

			char * rotulo2 = malloc(sizeof(char));
			*rotulo2 = (char)('A' + origem);
			insereListaFim(&graph.array[destino], rotulo2);
		}
	}

	// 4. Mostrar matriz de adjacência
	printf("\n===== Matriz de Adjacência =====\n\n   ");
	for (j = 0; j < graph.verticesQtd; j++)
		printf("%c ", (char)('A' + j));
	printf("\n");

	for (i = 0; i < graph.verticesQtd; i++) {
		printf("%c: ", (char)('A' + i));
		for (j = 0; j < graph.verticesQtd; j++)
			printf("%d ", matriz[i][j]);
		printf("\n");
	}

	// 5. Mostrar lista de adjacência (reaproveitando imprimirLista + imprimeChar)
	printf("\n===== Lista de Adjacência =====\n");
	for (i = 0; i < graph.verticesQtd; i++) {
		printf("%c: ", (char)('A' + i));
		imprimirLista(&graph.array[i], imprimeChar);
		printf("\n");
	}

	// 6. Vértice inicial para os percursos
	size_t inicio;
	printf("\nVértice inicial para DFS e BFS (0 a %zu): ", graph.verticesQtd - 1);
	if (scanf("%zu", &inicio) != 1 || inicio >= graph.verticesQtd) {
		printf("Vértice inválido, usando o vértice 0 como início.\n");
		inicio = 0;
	}

	// ===== DFS (busca em profundidade) =====
	printf("\n===== DFS a partir do vértice %c =====\n", (char)('A' + inicio));
	dfs(matriz, (uint32_t)graph.verticesQtd, (uint32_t)inicio);

	// ===== BFS (busca em largura) =====
	printf("\n===== BFS a partir do vértice %c =====\n", (char)('A' + inicio));
	bfs(matriz, (uint32_t)graph.verticesQtd, (uint32_t)inicio);

	// Liberação de memória
	for (i = 0; i < graph.verticesQtd; i++)
		liberarLista(&graph.array[i]);
	free(graph.array);

	for (i = 0; i < graph.verticesQtd; i++)
		free(matriz[i]);
	free(matriz);

	return 0;
}
