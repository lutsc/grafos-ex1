#include "bfs.h"

int32_t bfs(struct Graph * graph, uint32_t inicio) {
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

	// Fila da ordem de visitas
	uint32_t * fila = malloc(graph->verticesQtd * sizeof(uint32_t));
	if (fila == NULL) {
		free(fila);
		return 1;
	}

	uint32_t frente = 0, tras = 0;
	visitado[(inicio - 1)] = 1;
	fila[tras++] = (inicio - 1);

	printf("\nBFS a partir de %d: ", inicio);

	while (frente < tras) {
		uint32_t atual = fila[frente++];
		printf("%d ", (atual + 1));

		struct Node * temp = graph->array[atual].head;
		while (temp != NULL) {
			uint32_t vizinho = (temp->id - 1);
			if (!visitado[vizinho]) {
				visitado[vizinho] = 1;
				fila[tras++] = vizinho;
			}
			temp = temp->next;
		}
	}
	printf("\n");

	free(fila);
	free(visitado);

	return 0;
}