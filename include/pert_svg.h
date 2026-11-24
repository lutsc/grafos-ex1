#ifndef H_PERT_SVG
#define H_PERT_SVG 1

#include "pert.h"

/*
 * Desenha o grafo PERT/CPM na notação da aula e salva em um arquivo SVG.
 *
 * Cada atividade é um círculo com o número da atividade. Em volta dele:
 *     em cima:  início mínimo | duração | fim mínimo
 *     embaixo:  início máximo | fim máximo
 *     abaixo:   folga
 * As setas do caminho crítico são vermelhas. O vértice "Fim" fecha o grafo.
 *
 * Retorna 0 em sucesso e 1 em falha.
 */
int pertGerarSVG(const struct Pert * p, const char * arquivo);

#endif
