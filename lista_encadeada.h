#ifndef H_LISTA_ENCADEADA 
#define H_LISTA_ENCADEADA 1

struct Node
{
	void * data;
	Node * next;
};

/*
 * Insere dado no fim da lista, retorna 0 em sucesso e 1 em outros casos.
 */
int insereListaFim(Node * root, void * data);

/*
 * Remove o dado no fim da lista, retorna 0 em sucesso e 1 em outros casos.
 */
int removeListaFim(Node * root);

/*
 *  Busca um dado na lista e retorna o ponteiro do Nodo em *ret, a função
 *  retorna 0 caso encontre o dado e 1 caso contrário.
 */
int buscaLista(Node * root, void * data, Node * ret);

#endif
