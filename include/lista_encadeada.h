#ifndef H_LISTA_ENCADEADA
#define H_LISTA_ENCADEADA 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
	uint32_t id;
	uint32_t data;
	struct Node * next;
};

struct List {
	uint32_t id;
	uint32_t nodeQtd;
	struct Node * head;
};

/*
 * Inicializa a List
 */
int32_t iniciarLista(struct List * list, uint32_t id);

/*
 * Aloca memória para um node, retorna 0 em sucesso e 1 em falha de alocação.
 */
int32_t criarNode(struct Node ** node, uint32_t data, uint32_t id);

/*
 * Insere dado no fim da lista, retorna 0 em sucesso e 1 em outros casos.
 */
int32_t insereListaFim(struct List * list, struct Node ** node);

/*
 * Remove o dado no fim da lista, retorna 0 em sucesso e 1 em outros casos.
 */
int32_t removeListaFim(struct List * list, struct Node ** ret);

/*
 * Busca um dado na lista e retorna o ponteiro do Nodo em *ret, a função retorna 0 caso encontre o dado e 1 caso contrário.
 */
int32_t buscaLista(struct List * list, uint32_t data, struct Node ** ret); //TODO: Adicionar função para buscar usando id

/*
 * Libera memória de todos os nós de uma lista encadeada a partir da raiz
 */
void liberarLista(struct List * list);

/*
 * Imprime a lista encadeada
 */
void imprimirLista(struct List * list);

#endif
