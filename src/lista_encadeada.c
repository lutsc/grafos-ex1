#include <stdio.h>
#include <stdlib.h>
#include "lista_encadeada.h"

int32_t criarNode(struct Node ** node, void * data) {
	struct Node * newNode = malloc(sizeof(struct Node));
	if (newNode == NULL)
		return 1;

	newNode->data = data;
	newNode->next = NULL;

	*node = newNode;
	return 0;
}

int32_t insereListaFim(struct List * root, void * data) {
	if (root == NULL)
		return 1;

	struct Node * newNode = NULL;
	if (criarNode(&newNode, data) != 0)
		return 1;

	if (root->head == NULL) {
		root->head = newNode;
	} else {
		struct Node * t = root->head;
		while (t->next != NULL)
			t = t->next;
		t->next = newNode;
	}

	root->edgesQtd++;
	
	return 0;
}

int32_t removeListaFim(struct List * root) {
	if (root == NULL || root->head == NULL)
		return 1;

	struct Node * t1 = root->head;
	struct Node * t2 = t1->next;

	if (t2 == NULL) {
		root->head = NULL;
		free(t1);
		return 0;
	}

	while (t2->next != NULL) {
		t1 = t1->next;
		t2 = t2->next;
	}

	t1->next = NULL;
	free(t2->data);
	free(t2);
	root->edgesQtd--;

	return 0;
}

int32_t buscaLista(struct List * root, void * data, struct Node ** ret) {
	if (root == NULL)
		return 1;

	struct Node * atual = root->head;
	while (atual != NULL) {
		if (atual->data == data) {
			*ret = atual;
			return 0;
		}
		atual = atual->next;
	}
	return 1;
}

void imprimirLista(struct List * root, void (* printFunction)(void *)) {
	struct Node * atual = root->head;
	while (atual != NULL) {
		printFunction(atual->data);
		atual = atual->next;
	}
}

void liberarLista(struct List * root) {
	struct Node * atual = root->head;
	while (atual != NULL) {
		struct Node * prox = atual->next;
		free(atual->data);
		atual->data = NULL;
		free(atual);
		atual = prox;
	}
	root->head = NULL;
	root->edgesQtd = 0;
}


int32_t iniciarLista(struct List * root) {
	if (root == NULL)
		return 1;
 
	root->id = 0;
	root->edgesQtd = 0;
	root->head = NULL;
	return 0;
}


void imprimeChar(void * data) {
	printf("%c, ", *(char *)data);
}
