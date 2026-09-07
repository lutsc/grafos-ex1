#ifndef H_MATRIZES
#define H_MATRIZES 1

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Liga um nó a outro na matriz de adjacência
*/
int32_t inserirNaMatrizAdjacente(bool ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2);

uint32_t multiplicarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam);

uint32_t somarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam);

uint32_t liberarMatriz(bool ** mat, uint32_t tam);

uint32_t printMatriz(bool ** mat, uint32_t tam);

#endif