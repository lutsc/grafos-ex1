#include <stdio.h>
#include <stdlib.h>
#include "lista_encadeada.h"

int main() {
	int dirigido, verticesQtd;

	puts("\nO gráfico é dirigido?\n[1] Sim\n[0] Não");
	scanf("%d", &dirigido);
	if (dirigido != 0 && dirigido != 1) {
		printf("Opção inválida.\n");
		return 1;
	}
	
	printf("\nQuantos vértices terão em seu grafo: ");
	scanf("%d", &verticesQtd);
	if (verticesQtd <= 0) {
		printf("Opção inválida.\n");
		return 1;
	}
	
	printf("Dirigido: %d, Quantidade: %d\n", dirigido, verticesQtd);
	
	// Vetor de listas, uma struct List por vértice
	struct Graph graph;
	if (iniciarGrafo(&graph, verticesQtd) != 0) {
		return 1;
	}

	// // Valores teste para representar as conexões dos vértices
	// char conexoes1[] = {'A', 'B', 'C'};
	// char conexoes2[] = {'D', 'E', 'F'};
 
	// for (size_t i = 0; i < graph.verticesQtd; i++) {
	// 	char * temp = (char *)calloc(1, sizeof(char));
	// 	if (temp == NULL) {
	// 		printf("Erro ao alocar conexão.\n");
	// 		break;
	// 	}
	// 	*temp = conexoes1[i];
	// 	insereListaFim(&graph.array[0], temp);
 
	// 	temp = (char *)calloc(1, sizeof(char));
	// 	if (temp == NULL) {
	// 		printf("Erro ao alocar conexão.\n");
	// 		break;
	// 	}
	// 	*temp = conexoes2[i];
	// 	insereListaFim(&graph.array[1], temp);
	// }
 
	// // Verificando se os elementos tiveram suas conexões alocadas corretamente
	// if (graph.verticesQtd >= 2) {
	// 	printf("Conexões do %zu° elemento: ", graph.array[0].id);
	// 	imprimirLista(&graph.array[0], imprimeChar);
	// 	printf("\n");
	// 	printf("Conexões do %zu° elemento: ", graph.array[1].id);
	// 	imprimirLista(&graph.array[1], imprimeChar);
	// 	printf("\n");
	// }

	// Liberando listas
	for (size_t i = 0; i < (size_t)graph.verticesQtd; i++)
		liberarLista(&graph.array[i]);
	free(graph.array);

	return 0;
}
