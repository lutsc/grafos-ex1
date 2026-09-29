#ifndef H_MATRIZES
#define H_MATRIZES 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Liga um nó a outro na matriz de adjacência
*/
uint32_t inserirNaMatrizAdjacente(int32_t ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2);

uint32_t multiplicarMatrizes(int32_t ** mat1, int32_t ** mat2, int32_t ** matRet, uint32_t tam);

uint32_t somarMatrizes(int32_t ** mat1, int32_t ** mat2, int32_t ** matRet, uint32_t tam);

uint32_t liberarMatriz(int32_t ** mat, uint32_t tam);

uint32_t printMatriz(int32_t ** mat, uint32_t tam);

#endif
