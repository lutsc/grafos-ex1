#ifndef H_LISTA_ENCADEADA
#define H_LISTA_ENCADEADA 1
#include <stdbool.h>
#include <stdlib.h>

struct Node {
	size_t id;
	void * data;
	struct Node * next;
};

struct List {
	size_t id;
	size_t edgesQtd;
	struct Node * head;
};

struct Graph {
	size_t directed;
	size_t verticesQtd;
	struct List * array;
};

/*
 * Aloca memória para um node. Retorna 0 em sucesso e 1 em falha de alocação.
 */
int criarNode(struct Node ** node, void * data);

/*
 * Insere dado no fim da lista, retorna 0 em sucesso e 1 em outros casos.
 */
int insereListaFim(struct List * root, void * data);

/*
 * Remove o dado no fim da lista, retorna 0 em sucesso e 1 em outros casos.
 */
int removeListaFim(struct List * root);

/*
 *  Busca um dado na lista e retorna o ponteiro do Nodo em *ret, a função
 *  retorna 0 caso encontre o dado e 1 caso contrário.
 */
int buscaLista(struct List * root, void * data, struct Node ** ret);

/*
 *  Imprime a lista encadeada
 */
void imprimirLista(struct List * root, void (* printFunction)(void *));

/*
 * Libera memória de todos os nós de uma lista encadeada a partir da raiz
 */
void liberarLista(struct List * root);

/*
 * Inicializa a List
 */
int iniciarLista(struct List * root);

/*
 * Aloca o vetor de listas do grafo.
 * Retorna 0 em sucesso e 1 em falha de alocação.
 */
int iniciarGrafo(struct Graph * graph, int vertices);

/*
 * Imprime dados dos nós do tipo void * em tipo char
 */
void imprimeChar(void * data);

#endif
