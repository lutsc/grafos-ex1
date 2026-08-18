#include "grafos.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

int32_t gerarMatrizAdjacente(bool ***mat, uint32_t nos) {
	*mat = malloc(sizeof(bool*)*nos);
	for(uint32_t i = 0; i < nos; i++)
	{
		(*mat)[i] = malloc(sizeof(bool)*nos);
		for(uint32_t j = 0; j < nos; j++)
		{
			(*mat)[i][j] = 0;
		}
	}
	return 0;
}

int32_t inserirNaMatrizAdjacente(bool ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2)
{
	if (no1 >= tamanhoMatriz || no2 >= tamanhoMatriz)
		return 2;
	if(mat[no1][no2] == 1)
		return 1;
	mat[no1][no2] = 1;
	return 0;
}

int32_t dfs(bool ** mat, uint32_t tamanhoMatriz, uint32_t inicio) {
	if (inicio >= tamanhoMatriz)
		return 1;

	bool * visitado = calloc(tamanhoMatriz, sizeof(bool));
	uint32_t * pilha = malloc(tamanhoMatriz * sizeof(uint32_t));
	uint32_t topo = 0;

	pilha[topo] = inicio;
	while (topo >= 0) {
		uint32_t atual = pilha[topo--];

		if (!visitado[atual]) {
			visitado[atual] = true;
			printf("%c ", (char)('A' + atual)); //NOTE: Passar uma função de print como argumento

			// Empilha vizinhos em ordem reversa, para visitar em ordem crescente
			for (uint32_t j = tamanhoMatriz; j-- > 0; ) {
				if (mat[atual][j] && !visitado[j])
					pilha[++topo] = j;
			}
		}
	}
	printf("\n");

	free(pilha);
	free(visitado);
	return 0;
}

int32_t bfs(bool ** mat, uint32_t tamanhoMatriz, uint32_t inicio) {
	if (inicio >= tamanhoMatriz)
		return 1;

	bool * visitado = calloc(tamanhoMatriz, sizeof(bool));
	uint32_t * fila = malloc(tamanhoMatriz * sizeof(uint32_t));
	uint32_t frente = 0, tras = 0;

	visitado[inicio] = true;
	fila[tras++] = inicio;

	while (frente < tras) {
		uint32_t atual = fila[frente++];
		printf("%c ", (char)('A' + atual));

		for (uint32_t j = 0; j < tamanhoMatriz; j++) {
			if (mat[atual][j] && !visitado[j]) {
				visitado[j] = true;
				fila[tras++] = j;
			}
		}
	}
	printf("\n");

	free(fila);
	free(visitado);
	return 0;
}
