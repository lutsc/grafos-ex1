#include "grafos.h"

int32_t iniciarGrafo(struct Graph * graph, uint32_t vertices, bool dirigido) {
	if (graph == NULL || vertices <= 0)
		return 1;

	graph->verticesQtd = vertices;
	graph->directed = dirigido;
 
	graph->array = (struct List *)calloc(graph->verticesQtd, sizeof(struct List));
	if (graph->array == NULL) {
		printf("\nErro ao alocar memória para o vetor de listas.\n");
		return 1;
	}
 
	for (size_t i = 0; i < graph->verticesQtd; i++) {
		iniciarLista(&graph->array[i], (i + 1));
	}
 
	return 0;
}

int32_t liberaGrafo(struct Graph * graph) {
    if (graph == NULL || graph->array == NULL) {
        return 1;
	}

    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        liberarLista(&graph->array[i]);
	}

    free(graph->array);
    graph->array = NULL;
	graph->verticesQtd = 0;
	return 0;
}

// int32_t inserirAresta(struct Graph * graph, size_t id1, size_t id2){

// }

// int32_t inserirVertice(struct Graph * graph, struct Node ** node){

// }

// int32_t dfs(struct Graph ** graph, uint32_t inicio) {
	// if (inicio >= tamanhoMatriz)
	// 	return 1;

	// bool * visitado = calloc(tamanhoMatriz, sizeof(bool));
	// uint32_t * pilha = malloc(tamanhoMatriz * sizeof(uint32_t));
	// uint32_t topo = 0;

	// pilha[topo] = inicio;
	// while (topo >= 0) {
	// 	uint32_t atual = pilha[topo--];

	// 	if (!visitado[atual]) {
	// 		visitado[atual] = true;
	// 		printf("%c ", (char)('A' + atual)); //NOTE: Passar uma função de print como argumento

	// 		// Empilha vizinhos em ordem reversa, para visitar em ordem crescente
	// 		for (uint32_t j = tamanhoMatriz; j-- > 0; ) {
	// 			if (mat[atual][j] && !visitado[j])
	// 				pilha[++topo] = j;
	// 		}
	// 	}
	// }
	// printf("\n");

	// free(pilha);
	// free(visitado);
	// return 0;
// }

// int32_t bfs(struct Graph ** graph, uint32_t inicio) {
	// if (inicio >= tamanhoMatriz)
	// 	return 1;

	// bool * visitado = calloc(tamanhoMatriz, sizeof(bool));
	// uint32_t * fila = malloc(tamanhoMatriz * sizeof(uint32_t));
	// uint32_t frente = 0, tras = 0;

	// visitado[inicio] = true;
	// fila[tras++] = inicio;

	// while (frente < tras) {
	// 	uint32_t atual = fila[frente++];
	// 	printf("%c ", (char)('A' + atual));

	// 	for (uint32_t j = 0; j < tamanhoMatriz; j++) {
	// 		if (mat[atual][j] && !visitado[j]) {
	// 			visitado[j] = true;
	// 			fila[tras++] = j;
	// 		}
	// 	}
	// }
	// printf("\n");

	// free(fila);
	// free(visitado);
	// return 0;
// }

int32_t gerarMatrizAdjacente(struct Graph * graph, bool ***mat) {
	*mat = malloc(sizeof(bool*) * graph->verticesQtd);
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		(*mat)[i] = calloc(graph->verticesQtd, sizeof(bool));
		for(uint32_t j = 0; j < graph->verticesQtd; j++) {
			(*mat)[i][j] = 0;
		}
	}
	return 0;
}

int32_t inserirNaMatrizAdjacente(bool ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2) {
	if (no1 >= tamanhoMatriz || no2 >= tamanhoMatriz)
		return 2;
	if(mat[no1][no2] == 1)
		return 1;
	mat[no1][no2] = 1;
	return 0;
}

int32_t ftd(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdA[tamanhoMatriz])
{
	int32_t result[tamanhoMatriz] = {};
	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		result[i] = -1;
	}

	uint32_t current;
	uint32_t * stack = calloc(tamanhoMatriz, sizeof(uint32_t));
	uint32_t top = 0;

	stack[0] = node;
	current = node;
	result[node] = 0;
	bool change;

	while(true)
	{
		change = false;
		for(uint32_t i = 0; i < tamanhoMatriz; i++)
		{
			if(mat[current][i] > 0)
			{
				if(result[i] == -1)
				{
					result[i] = result[current]+1;
					stack[++top] = i; 
					change = true;
				}
			}

		}
		current = stack[top];
		top--;
		if(!change)
			break;
	}
	free(stack);

	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		ftdA[i] = result[i];
	}

	return 0;
}

int32_t ftdi(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdiA[tamanhoMatriz])
{
	int32_t result[tamanhoMatriz] = {};
	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		result[i] = -1;
	}

	uint32_t current;
	uint32_t * stack = calloc(tamanhoMatriz, sizeof(uint32_t));
	uint32_t top = 0;

	stack[0] = node;
	current = node;
	result[node] = 0;
	bool change;

	while(true)
	{
		change = false;
		for(uint32_t i = 0; i < tamanhoMatriz; i++)
		{
			if(mat[i][current] > 0)
			{
				if(result[i] == -1)
				{
					result[i] = result[current]+1;
					stack[++top] = i; 
					change = true;
				}
			}

		}
		current = stack[top];
		top--;
		if(!change)
			break;
	}
	free(stack);

	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		ftdiA[i] = result[i];
	}

	return 0;
}

