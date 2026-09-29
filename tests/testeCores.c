#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"
#include "coloracao.h"


void mostrarCores(int * ret, int size1, int * ref, int size2);

int main()
{
	struct Graph graph;
	iniciarGrafo(&graph, 5, 0);

	inserirAresta(&graph, 1, 2, 1);
	inserirAresta(&graph, 1, 3, 1);
	inserirAresta(&graph, 1, 4, 1);

	inserirAresta(&graph, 3, 2, 1);
	inserirAresta(&graph, 3, 4, 1);

	inserirAresta(&graph, 2, 4, 1);

	inserirAresta(&graph, 5, 2, 1);
	inserirAresta(&graph, 5, 4, 1);

	mostrarGrafo(&graph);

	int32_t ret[5] = {0};
	int32_t ref[5] = {2, 1, 3, 4, 2};
	maiorGrauColoracao(&graph, ref, ret);

	mostrarCores(ret, 5, ref, 5);

}

void mostrarCores(int * ret, int size1, int * ref, int size2) {
	printf("Ret: ");
	for(int i = 0; i < size1; i++)
	{
		printf("%d ", ret[i]);
	}
	puts("");

	printf("Ref: ");
	for(int i = 0; i < size2; i++)
	{
		printf("%d ", ref[i]);
	}
	puts("");
}

