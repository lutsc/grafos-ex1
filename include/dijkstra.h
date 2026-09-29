#ifndef H_DIJKSTRA
#define H_DIJKSTRA 1

#include <stdint.h>
#include <grafos.h>

/*
 * Recebe um grafo e retorna o vetor de estimativas de distância e de vértice anterior do algoritmo de dijkstra
 */
int32_t dijkstra(struct Graph * graph, uint32_t vertice, int32_t estimate[], int32_t previous[]);


#endif
