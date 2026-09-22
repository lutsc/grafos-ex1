#include "coloracao.h"

int32_t verticesColoracao(struct Graph * graph, int *ret) {

	int vertices = graph->array->nodeQtd;

	if (vertices <= 0)
		return 0;
	
	int saturacao[vertices];
	int coloracao[vertices];

	int maiorGrau;

}