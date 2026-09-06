#include "matrizes.h"

int32_t multiplicarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam){
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

int32_t somarMatrizes(bool ** mat1, bool ** mat2, bool ** matRet, uint32_t tam){
	if(mat1 == NULL || mat2 == NULL || tam <= 0)
		return 1;

	for(uint32_t i=0; i<tam; i++){
		for(uint32_t j=0; j<tam; j++){
			matRet[i][j] += (mat1[i][j] + mat2[i][j]);
		}
	}
	return 0;
}

int32_t printMatriz(bool ** mat, uint32_t tam){
	if(mat == NULL || tam <= 0)
		return 1;

	for(uint32_t i=0; i<tam; i++){
		for(uint32_t j=0; j<tam; j++){
			printf("%d ", mat[i][j]);
		}	
		printf("\n");
	}
	return 0;
}
