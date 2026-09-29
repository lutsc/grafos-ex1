#include "dijkstra.h"
#include "grafos.h"

int32_t dijkstra(struct Graph * graph, uint32_t vertice, int32_t estimate[], int32_t previous[]) {
	if(graph == NULL)
		return -1; //Grafo inexistente

	if(graph->verticesQtd == 0)
		return -1; //Grafo vazio


	bool closed[graph->verticesQtd];
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		estimate[i] = -1;
		previous[i] = -1;
		closed[i] = 0;
	}

	estimate[vertice] = 0;
	previous[vertice] = vertice;





	

	return 0;
}

