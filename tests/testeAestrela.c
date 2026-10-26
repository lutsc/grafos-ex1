#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"
#include "coloracao.h"
#include "busca.h"

void mostraVetor (int32_t vet[], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%3d ", vet[i]);
	}
	puts("");
}

void mostrarGrafo(struct Graph * graph);
void mostrarCores(int * ret, int size1, int * ref, int size2);

int main()
{
	int32_t est[7], prev[7];
	int32_t heuristicZero[7] = {};
	int32_t heuristic[7] = {30, 26, 21, 7, 22, 36, 0};

	struct Graph graph;
	iniciarGrafo(&graph, 7, 1); 

	inserirAresta(&graph, 1, 2, 12);
	inserirAresta(&graph, 1, 3, 14);

	inserirAresta(&graph, 2, 3, 9);
	inserirAresta(&graph, 2, 4, 38);

	inserirAresta(&graph, 3, 4, 24);
	inserirAresta(&graph, 3, 5, 7);

	inserirAresta(&graph, 5, 4, 13);
	inserirAresta(&graph, 5, 6, 29);
	inserirAresta(&graph, 5, 7, 9);

	mostrarGrafo(&graph);

	if(!aestrela(&graph, 0, 6, heuristicZero, est, prev))
	{
		puts("A* com heurística zerada:");
		printf("Est: ");mostraVetor(est, 7);
		printf("Prev: ");mostraVetor(prev, 7);
	}

	puts("");
	if(!dijkstra(&graph, 0, est, prev))
	{
		puts("Dijkstra:");
		printf("Est: ");mostraVetor(est, 7);
		printf("Prev: ");mostraVetor(prev, 7);
	}

	puts("");
	if(!aestrela(&graph, 0, 6, heuristic, est, prev))
	{
		puts("A* com heurística:");
		printf("Est: ");mostraVetor(est, 7);
		printf("Prev: ");mostraVetor(prev, 7);
	}
}
