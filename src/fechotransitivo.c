#include "fechotransitivo.h"

/*
 * Retorna o fecho transitivo direto do node passado na função
*/
int32_t ftd(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdA[tamanhoMatriz]) {
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

int32_t ftdGrafo(struct Graph * graph, uint32_t v, int32_t * ftdA) {
	if (graph == NULL || graph->array == NULL || v >= graph->verticesQtd) {
		return 1;
	}

	bool ** mat = NULL;
	gerarMatrizAdjacente(graph, &mat);

	int32_t ret = ftd(mat, graph->verticesQtd, v, ftdA);
	liberarMatriz(mat, graph->verticesQtd);

	return ret;
}

/*
 * Retorna o fecho transitivo inverso do node passado na função
*/
int32_t ftdi(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdiA[tamanhoMatriz]) {
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

int32_t ftdiGrafo(struct Graph * graph, uint32_t v, int32_t * ftdiA) {
	if (graph == NULL || graph->array == NULL || v >= graph->verticesQtd) {
		return 1;
	}

	bool ** mat = NULL;
	gerarMatrizAdjacente(graph, &mat);

	int32_t ret = ftdi(mat, graph->verticesQtd, v, ftdiA);
	liberarMatriz(mat, graph->verticesQtd);

	return ret;
}

/*
 * Retorna se a o grafo é conexo(1) ou não(0)
*/
int32_t eConexo(bool ** mat, uint32_t tamanhoMatriz)
{
	int32_t ftdA[tamanhoMatriz] = {};
	int32_t ftdiA[tamanhoMatriz] = {};

	ftd(mat, tamanhoMatriz, 0, ftdA);
	ftdi(mat, tamanhoMatriz, 0, ftdiA);

	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		if(ftdA[i] == -1 || ftdiA[i] == -1)
		{
			return 0;
		}
	}
	return 1;
}

int32_t grafoConexo(struct Graph * graph) {
	if (graph == NULL || graph->array == NULL || graph->verticesQtd <= 0) {
		return 1;
	}

	bool ** mat = NULL;
	gerarMatrizAdjacente(graph, &mat);

	int32_t ret = eConexo(mat, graph->verticesQtd);
	liberarMatriz(mat, graph->verticesQtd);
	return ret;
}