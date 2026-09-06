#ifndef H_GRAFOS
#define H_GRAFOS 1
#include <stdint.h>
#include <stdbool.h>

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

/*
 * Retorna o fecho transitivo direto do node passado na função
*/
int32_t ftd(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, bool ftd[tamanhoMatriz]);

/*
 * Retorna o fecho transitivo direto inverso do node passado na função
*/
int32_t ftdi(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, bool ftdi[tamanhoMatriz]);

#endif
