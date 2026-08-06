#include <stdlib.h>
#include "lista_encadeada.h"

int criarNode(struct Node ** node, void * data) {
	struct Node * newNode = malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;

	*node = newNode;
	return 0;
}

int insereListaFim(struct Node ** root, void * data) {
	struct Node * newNode = NULL;
	criarNode(&newNode, data);

	if (*root == NULL) 
	{
		*root = newNode;
	}
	else {
		struct Node * t = *root;
		while(t->next != NULL) 
			t = t->next;

		t->next = newNode;
	}
	return 0;
}

int removeListaFim(struct Node ** root) {
	if (*root == NULL) 
		return 1;
	struct Node * t1 = *root;
	struct Node * t2 = (*root)->next;
	if(t1->next == NULL) {
		*root = NULL;
		free(t1);
		return 0;
	}
	while(t2->next != NULL) {
		t1 = t1->next;
		t2 = t2->next;
	}
	t1->next = NULL;
	free(t2);
	return 0;
}

int buscaLista(struct Node * root, void * data, struct Node ** ret) {
	if (root == NULL)
		return 1;
	while (root->next != NULL)
	{
		if (root->data == data)
		{
			*ret = root;
			return 0;
		}
		root = root->next;
	}
	return 1;
}

void imprimirLista(struct Node * root, void (* printFunction)(void *)) { 
	while(root != NULL)
	{
		printFunction(root->data);
		root = root->next;
	}
}



