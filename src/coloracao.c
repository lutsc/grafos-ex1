#include "coloracao.h"

int32_t verticesColoracao(struct Graph * graph, int *ret) {

	int vertices = graph->array->nodeQtd;

	if (vertices <= 0)
		return 0;
	
	int saturacao[vertices];
	int coloracao[vertices];

	int maiorGrau;

}

int32_t maxVetor(int32_t * vec, uint32_t size, int32_t * max) {
	if(vec == NULL)
		return -1;
	if(size == 0)
		return -1;

	int32_t temp = 0;
	*max = vec[0];

	for(uint32_t i = 0; i < size; i++){
		temp = vec[i];
		if(temp > *max)
			*max = vec[temp];
	}

	return 0;
}


int32_t grausColoracao(struct Graph * graph, int32_t *ref, int32_t *ret) {

	int32_t **mat = NULL;
	bool usedColors[graph->verticesQtd]; 

	if(gerarMatrizAdjacente(graph, &mat))
		return -1;

	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		ret[i] = 0;
	}

	for(uint32_t i = 0; i < graph->verticesQtd; i++) {

		// if(ref[i] == 0) 
		// 	continue;

		for(uint32_t k = 0; k < graph->verticesQtd; k++)
			usedColors[k] = false;

		for(uint32_t j = 0; j < graph->verticesQtd; j++) {
			if(mat[i][j] > 0 && ref[j] != 0) {
				if(!usedColors[ref[j]-1]) {
					usedColors[ref[j]-1] = true;
					ret[i]++;
				}
			}
		}
	}
	return 0;
}

int32_t verticesColoracaoGrafo(struct Graph * graph, int32_t *ret) {
	if(graph == NULL)
		return -1;

	if(graph->verticesQtd == 0)
		return -1;

	int32_t **mat = NULL;

	if(gerarMatrizAdjacente(graph, &mat))
		return -1;

	uint32_t temp = 0;
	uint32_t maiorGrau = 0;
	uint32_t indiceMaiorCor = 0;
	bool usedColors[graph->verticesQtd];

	for(uint32_t i = 0; i < graph->verticesQtd; i++){
		ret[i] = 0;
		usedColors[i] = false;

		temp = graph->array[i].nodeQtd;
		if(temp > graph->array[maiorGrau].nodeQtd)
			maiorGrau = i;
	}
	ret[maiorGrau] = 1;

	int32_t grausCor[graph->verticesQtd];
	uint32_t current = indiceMaiorCor; 
	uint32_t tempColor = 0; 

	bool done = false;


	while(!done) {
		done = true;

		grausColoracao(graph, ret, grausCor);

		indiceMaiorCor = 0;
		for(uint32_t j = 0; j < graph->verticesQtd; j++){
			usedColors[j] = false;
			grausCor[j] = (uint32_t)grausCor[j] & ((bool)ret[j] - 1);

			if(grausCor[j] > grausCor[(int32_t)indiceMaiorCor])
				indiceMaiorCor = j;
		}
		current = indiceMaiorCor;

		for(uint32_t j = 0; j < graph->verticesQtd; j++){
			if(mat[current][j] > 0 && ret[j] > 0) {
				usedColors[ret[j]-1] = true;
			}
			if(ret[j] == 0)
				done = false;
		}

		for(uint32_t j = 0; j < graph->verticesQtd; j++){
			if(!usedColors[j]) {
				ret[current] = j+1;
				break;
			}
		}

	}


	liberarMatriz(mat, graph->verticesQtd);
	return 0;
}
