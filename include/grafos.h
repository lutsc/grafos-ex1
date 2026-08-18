#ifndef H_GRAFOS
#define H_GRAFOS 1
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "lista_encadeada.h"

struct Graph {
	size_t directed;
	size_t verticesQtd;
	struct List * array; //Lista de adjacência
};

/*
 * Aloca o vetor de listas do grafo.
 * Retorna 0 em sucesso e 1 em falha de alocação.
 */
int32_t iniciarGrafo(struct Graph * graph, uint32_t vertices, bool dirigido);

//TODO: Comentar
int32_t inserirArco(struct Graph * graph, size_t id1, size_t id2);

/*
 * Gera a matriz de adjacência para uma determinada quantidade de nós
*/
int32_t gerarMatrizAdjacente(bool ***mat, uint32_t nos);

/*
 * Liga um nó a outro na matriz de adjacência
*/
int32_t inserirNaMatrizAdjacente(bool ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2);

/*
 * Percorre o grafo em profundidade (DFS) a partir do vértice "inicio",
 * imprimindo a ordem de visita (vértices rotulados de A a Z).
 * Retorna 0 em sucesso e 1 caso "inicio" seja inválido.
*/
int32_t dfs(bool ** mat, uint32_t tamanhoMatriz, uint32_t inicio);

/*
 * Percorre o grafo em largura (BFS) a partir do vértice "inicio",
 * imprimindo a ordem de visita (vértices rotulados de A a Z).
 * Retorna 0 em sucesso e 1 caso "inicio" seja inválido.
*/
int32_t bfs(bool ** mat, uint32_t tamanhoMatriz, uint32_t inicio);

#endif
