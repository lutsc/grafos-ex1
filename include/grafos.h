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
int32_t preencherMatrizAdjacente(bool ** mat, uint32_t tamanhoMatriz, uint32_t no1, uint32_t no2);


#endif
