#ifndef H_COLORACAO
#define H_COLORACAO 1

#include "matrizes.h"
#include "grafos.h"

int32_t verticesColoracao(struct Graph * graph, int *ret);

/*
 * Modifica a variavel *max para adotar o maior função de um vetor, retorna 0 em êxito e -1 caso contrário
 */
int32_t maxVetor(int32_t * vec, uint32_t size, int32_t * max);

/*
 * Modifica o array ret para retornar os graus de coloração do vértice baseado no array de cores ref,
 * retorna 0 em êxito e -1 caso contrário
 */
int32_t grausColoracao(struct Graph * graph, int32_t *ref, int32_t *ret);

/*
 * Modifica o array ret para retornar as cores dos vértices, retorna 0 em êxito e -1 caso contrário
 */
int32_t verticesColoracaoGrafo(struct Graph * graph, int32_t *ret);

#endif
