#ifndef H_MATRIZES
#define H_MATRIZES 1

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int32_t multiplicarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam);

int32_t somarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam);

int32_t printMatriz(bool ** mat, uint32_t tam);

#endif