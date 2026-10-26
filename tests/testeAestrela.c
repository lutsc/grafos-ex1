#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"
#include "coloracao.h"
#include "dijkstra.h"

void mostraVetor (int32_t vet[], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", vet[i]);
	}
	puts("");
}
/*
 * Recebe um grafo e retorna o vetor de estimativas de distância e de vértice anterior do algoritmo de dijkstra
 */
int32_t dijkstra(struct Graph * graph, uint32_t vertice, int32_t estimate[], int32_t previous[]);

void mostrarGrafo(struct Graph * graph);
void mostrarCores(int * ret, int size1, int * ref, int size2);

int main()
{
	int32_t est[5], prev[5];

	struct Graph graph;
	iniciarGrafo(&graph, 5, 1); 

	inserirAresta(&graph, 1, 2, 1);
	inserirAresta(&graph, 1, 3, 5);

	inserirAresta(&graph, 2, 5, 5);
	inserirAresta(&graph, 2, 3, 1);

	inserirAresta(&graph, 3, 4, 1);

	inserirAresta(&graph, 4, 2, 3);
	inserirAresta(&graph, 4, 5, 1);

	mostrarGrafo(&graph);

	if(!dijkstra(&graph, 0, est, prev))
	{
		puts("Dijkstra:");
		printf("Est: ");mostraVetor(est, 5);
		printf("Prev: ");mostraVetor(prev, 5);
	}

}
