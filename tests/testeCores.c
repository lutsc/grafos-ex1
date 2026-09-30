#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"
#include "coloracao.h"

#define qtdVertices 5


void mostrarCores(int * ret, int size1, int * ref, int size2);

int main()
{
	struct Graph graph;
	iniciarGrafo(&graph, qtdVertices, 1);

	inserirAresta(&graph, 1, 2, 1);
	inserirAresta(&graph, 1, 3, 1);
	inserirAresta(&graph, 1, 4, 1);

	inserirAresta(&graph, 3, 2, 1);
	inserirAresta(&graph, 3, 4, 1);

	inserirAresta(&graph, 2, 4, 1);

	inserirAresta(&graph, 5, 2, 1);
	inserirAresta(&graph, 5, 4, 1);

	mostrarGrafo(&graph);

	printf("\n\t---\t Teste graus de coloração --- \t\n");
	int32_t ret[qtdVertices] = {0};
	// int32_t ref[5] = {0, 1, 0, 0, 0};
	int32_t ref[qtdVertices] = {2, 1, 3, 4, 2};
	grausColoracao(&graph, ref, ret);
	mostrarCores(ret, qtdVertices, ref, qtdVertices);

	printf("\n\t---\t Teste cores para os vértices --- \t\n");
	verticesColoracaoGrafo(&graph, ret);
	printf("Ret: ");
	for(int i = 0; i < qtdVertices; i++)
	{
		printf("%d ", ret[i]);
	}
	puts("");
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
