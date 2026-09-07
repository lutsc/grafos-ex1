#include "matrizes.h"

uint32_t inserirNaMatrizAdjacente(bool ** mat, uint32_t tam, uint32_t no1, uint32_t no2) {
	if (no1 >= tam || no2 >= tam)
		return 2;
	if(mat[no1][no2] == 1)
		return 1;
	mat[no1][no2] = 1;
	return 0;
}

uint32_t multiplicarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam){
	if(mat1 == NULL || mat2 == NULL || tam <= 0)
		return 1;

	for(uint32_t i=0; i<tam; i++){
		for(uint32_t j=0; j<tam; j++){
			for(uint32_t k=0; k<tam; k++){
				matRet[i][j] += (mat1[i][k] * mat2[k][j]);
			}
		}
	}
	return 0;
}

uint32_t somarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam){
	if(mat1 == NULL || mat2 == NULL || tam <= 0)
		return 1;

	for(uint32_t i=0; i<tam; i++){
		for(uint32_t j=0; j<tam; j++){
			matRet[i][j] += (mat1[i][j] + mat2[i][j]);
		}
	}
	return 0;
}

uint32_t liberarMatriz(bool ** mat, uint32_t tam) {
	if (mat == NULL) {
		return 1;
	}

	for (uint32_t i = 0; i < tam; i++) {
		free(mat[i]);
	}
	free(mat);

	return 0;
}

uint32_t printMatriz(bool ** mat, uint32_t tam){
	if(mat == NULL || tam <= 0) {
		return 1;
	}

	for(uint32_t i=0; i<tam; i++){
		for(uint32_t j=0; j<tam; j++){
			printf("%d ", mat[i][j]);
		}	
		printf("\n");
	}
	return 0;
}
