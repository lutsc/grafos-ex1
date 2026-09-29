#include "dijkstra.h"
#include "grafos.h"

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
			if(!closed[i]) {
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

