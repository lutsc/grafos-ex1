#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"
#include "coloracao.h"


void mostrarGrafo(struct Graph * graph);
void mostrarCores(int * ret, int size1, int * ref, int size2);

int main()
{
	struct Graph graph;
	iniciarGrafo(&graph, 5, 0);

	inserirAresta(&graph, 1, 2);
	inserirAresta(&graph, 1, 3);
	inserirAresta(&graph, 1, 4);

	inserirAresta(&graph, 3, 2);
	inserirAresta(&graph, 3, 4);

	// inserirAresta(&graph, 2, 4);

	inserirAresta(&graph, 5, 2);
	inserirAresta(&graph, 5, 4);

	mostrarGrafo(&graph);

	int32_t ret[5] = {0};
	int32_t ref[5] = {1, 1, 1, 1, 0};
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


void mostrarGrafo(struct Graph * graph) {
    if (graph == NULL || graph->array == NULL) {
        printf("Grafo inválido ou vazio.\n");
        return;
    }
    
    printf("\n- LISTA DE ADJACÊNCIA -\n");
    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        printf("%d -> ", i + 1);
        struct List * atual = &graph->array[i];
        if (atual == NULL) {
            printf("NULL");
        }
		imprimirLista(atual);
        printf("\n");
    }
    
    printf("\n- MATRIZ DE ADJACÊNCIA -\n");
    int32_t ** mat = NULL;
    if (gerarMatrizAdjacente(graph, &mat) == 0) {
        printf("   ");
        for (uint32_t i = 0; i < graph->verticesQtd; i++) {
            printf(" %d ", i + 1);
        }
        printf("\n");
        
        for (uint32_t i = 0; i < graph->verticesQtd; i++) {
            printf(" %d ", i + 1);
            for (uint32_t j = 0; j < graph->verticesQtd; j++) {
                printf(" %d ", mat[i][j]);
            }
            printf("\n");
        }
        
        liberarMatriz(mat, graph->verticesQtd);
    }
}
