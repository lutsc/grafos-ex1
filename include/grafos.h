#ifndef H_GRAFOS
#define H_GRAFOS 1

#include "lista_encadeada.h"
#include "matrizes.h"

#include <sys/types.h>

struct Graph {
	bool directed;
	uint32_t verticesQtd;
	struct List * array; //Lista de adjacência
};

/*
 * Aloca o vetor de listas do grafo.
 * Retorna 0 em sucesso e 1 em falha de alocação.
 */
int32_t iniciarGrafo(struct Graph * graph, uint32_t vertices, bool dirigido);

/*
 * Libera memória de todos o conteúdo do grafo
 */
int32_t liberaGrafo(struct Graph * graph);

/*
 * Cria uma conexão entre dois nós usando os seus ids
 */
int32_t inserirAresta(struct Graph * graph, uint32_t id1, uint32_t id2);

/*
 * Desfaz uma conexão entre dois nós usando os seus ids
 */
int32_t removerAresta(struct Graph * graph, uint32_t id1, uint32_t id2);

/*
 * Adiciona um vértice em um grafo já existente
 * Implementar id? ordem das vértices não influencia nas conexões
 */
int32_t inserirVertice(struct Graph * graph);

/*
 * Remove um vértice do id dado em um grafo já existente
 */
int32_t removerVertice(struct Graph * graph, uint32_t id);

/*
 * Percorre o grafo em profundidade (DFS) a partir do vértice "inicio", imprimindo a ordem de visita
 * Retorna 0 em sucesso e 1 caso "inicio" seja inválido.
*/
int32_t dfs(struct Graph * graph, uint32_t inicio);

/*
 * Percorre o grafo em largura (BFS) a partir do vértice "inicio",
 * imprimindo a ordem de visita (vértices rotulados de A a Z).
 * Retorna 0 em sucesso e 1 caso "inicio" seja inválido.
*/
int32_t bfs(struct Graph * graph, uint32_t inicio);

/*
 * Gera a matriz de adjacência para uma determinada quantidade de nós
*/
int32_t gerarMatrizAdjacente(struct Graph * graph, bool ***mat);

/*
 * Retorna o fecho transitivo direto do node passado na função
*/
int32_t ftd(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftd[tamanhoMatriz]);

/*
 * Retorna o fecho transitivo direto inverso do node passado na função
*/
int32_t ftdi(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdi[tamanhoMatriz]);


#endif
