#include "coloracao.h"

int32_t verticesColoracao(struct Graph * graph, int *ret) {

	int vertices = graph->array->nodeQtd;

	if (vertices <= 0)
		return 0;
	
	int saturacao[vertices];
	int coloracao[vertices];

	int maiorGrau;

}

int32_t maiorGrauColoracao(struct Graph * graph, int32_t *ref, int32_t *ret) {

	bool **mat = NULL;

	if(gerarMatrizAdjacente(graph, &mat))
		return -1;

	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		ret[i] = 0;
	}

	for(uint32_t i = 0; i < graph->verticesQtd; i++) {

		if(ref[i] == 0) //TODO: Ainda não está conferindo se um vértice está adjacente a duas cores iguais
			continue;

		for(uint32_t j = 0; j < graph->verticesQtd; j++) {
			if(mat[i][j] > 0)
				ret[j]++;
		}
	}
	return 0;
}

int32_t verticesColoracaoGrafo(struct Graph * graph, int32_t *ret) {

	bool **mat = NULL;

	if(gerarMatrizAdjacente(graph, &mat))
		return -1;


	uint32_t temp = 0;
	uint32_t maiorGrau = 0;
	for(uint32_t i = 0; i < graph->verticesQtd; i++){
		ret[i] = -1;

		temp = graph->array[i].nodeQtd;
		if(temp > maiorGrau)
			maiorGrau = i;
	}


	ret[maiorGrau] = 1;
	while(true){
	}


	liberarMatriz(mat, graph->verticesQtd);
	return 0;
}
