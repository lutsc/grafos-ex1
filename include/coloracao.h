#ifndef H_COLORACAO
#define H_COLORACAO 1

#include "grafos.h"

int32_t verticesColoracao(struct Graph * graph, int *ret);

int32_t maiorGrauColoracao(struct Graph * graph, int32_t *ref, int32_t *ret);
int32_t verticesColoracaoGrafo(struct Graph * graph, int32_t *ret);

#endif
