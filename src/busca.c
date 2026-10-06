#include "busca.h"

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

int32_t dijkstra(struct Graph * graph, uint32_t vertice, int32_t estimate[], int32_t previous[]) {
	if(graph == NULL)
		return -1; //Grafo inexistente

	if(graph->verticesQtd == 0)
		return -1; //Grafo vazio

	int32_t **mat;
	gerarMatrizAdjacente(graph, &mat);

	bool closed[graph->verticesQtd];
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		estimate[i] = -1;
		previous[i] = -1;
		closed[i] = 0;
	}

	estimate[vertice] = 0;
	previous[vertice] = vertice;

	uint32_t current = vertice;
	uint32_t min = current;

	while(true)
	{
		for(uint32_t i = 0; i < graph->verticesQtd; i++) {
			if(mat[current][i] > 0 && !closed[i]) 
					if(estimate[i] == -1 || (estimate[current]+mat[current][i] < estimate[i] && estimate[i] != -1)){
						estimate[i] = estimate[current] + mat[current][i];
						previous[i] = current;
					}
		}
		closed[current] = 1;

		for(uint32_t i = 0; i < graph->verticesQtd; i++) {
			if(!closed[i] && estimate[i] != -1) {
				min = i;
				break;
			}
		}

		for(uint32_t i = 0; i < graph->verticesQtd; i++) {
			if((estimate[i] < estimate[min] && !closed[i] && estimate[i] != -1))
				min = i;
		}

		if(min == current)
			break;

		current = min;
	}


	return 0;
}
