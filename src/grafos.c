#include "grafos.h"
#include <stdlib.h>

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

int32_t preencherMatrizAdjacente(bool ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2)
{
	if (no1 >= tamanhoMatriz || no2 >= tamanhoMatriz)
		return 2;
	if(mat[no1][no2] == 1)
		return 1;
	mat[no1][no2] = 1;
	return 0;
}


