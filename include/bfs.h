#ifndef H_BFS
#define H_BFS 1

#include "grafos.h"

/*
 * Percorre o grafo em largura (BFS) a partir do vértice "inicio",
 * imprimindo a ordem de visita (vértices rotulados de A a Z).
 * Retorna 0 em sucesso e 1 caso "inicio" seja inválido.
*/
int32_t bfs(struct Graph * graph, uint32_t inicio);

#endif