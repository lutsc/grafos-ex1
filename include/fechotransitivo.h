#ifndef H_FECHOTRANSITIVO
#define H_FECHOTRANSITIVO 1

#include "grafos.h"

/*
 * Retorna o fecho transitivo direto do node passado na função
*/
int32_t ftd(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdA[tamanhoMatriz]);
int32_t ftdGrafo(struct Graph * graph, uint32_t v, int32_t * ftdA);

/*
 * Retorna o fecho transitivo inverso do node passado na função
*/
int32_t ftdi(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdiA[tamanhoMatriz]);
int32_t ftdiGrafo(struct Graph * graph, uint32_t v, int32_t * ftdiA);

/*
 * Retorna se a o grafo é conexo(1) ou não(0)
*/
int32_t eConexo(bool ** mat, uint32_t tamanhoMatriz);
int32_t grafoConexo(struct Graph * graph);

#endif