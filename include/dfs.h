#ifndef H_DFS
#define H_DFS 1

#include "grafos.h"

/*
 * Percorre o grafo em profundidade (DFS) a partir do vértice "inicio", imprimindo a ordem de visita
 * Retorna 0 em sucesso e 1 caso "inicio" seja inválido.
*/
int32_t dfs(struct Graph * graph, uint32_t inicio);

#endif