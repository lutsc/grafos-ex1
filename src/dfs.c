#include "dfs.h"

int32_t dfs(struct Graph * graph, uint32_t inicio) {
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

	if (inicio <= 0 || inicio > graph->verticesQtd){
		// Vértice inválido
		return 1;
	}

	// Array de vértices visitados
	bool * visitado = (bool *)calloc(graph->verticesQtd, sizeof(bool));
	if (visitado == NULL) {
		free(visitado);
		return 1;
	}

	// Pilha da ordem de visitas
	uint32_t * pilha = malloc(graph->verticesQtd * sizeof(uint32_t));
	if (pilha == NULL) {
		free(visitado);
		return 1;
	}

	int32_t topo = 0;
	pilha[topo] = (inicio - 1);
	printf("\nDFS a partir de %d: ", inicio);

	while (topo >= 0) {
		// Desempilha elemento
		uint32_t atual = pilha[topo--];

		if (!visitado[atual]) {
			visitado[atual] = 1;
			printf("%d ", (atual + 1));

			// Conta os vizinhos do vértice atual
			struct Node * temp = graph->array[atual].head;
			uint32_t vizinhosQtd = 0;
			while (temp != NULL) {
				vizinhosQtd++;
				temp = temp->next;
			}

			// Empilha vizinhos
			if (vizinhosQtd > 0) {
				uint32_t * vizinhos = (uint32_t *)malloc(vizinhosQtd * sizeof(uint32_t));
				if (vizinhos != NULL) {
					temp = graph->array[atual].head;
					for (uint32_t i = 0; i < vizinhosQtd; i++) {
						vizinhos[i] = (temp->id - 1);
						temp = temp->next;
					}

					for (int32_t i = ((int32_t)vizinhosQtd - 1); i >= 0; i--) {
						uint32_t vizinho = vizinhos[i];
						if (!visitado[vizinho]) {
							pilha[++topo] = vizinho;
						}
					}
					free(vizinhos);
				}
			}
		}
	}
	printf("\n");

	free(pilha);
	free(visitado);

	return 0;
}