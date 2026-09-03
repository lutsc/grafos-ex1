#include "lista_encadeada.h"

int32_t iniciarLista(struct List * list, uint32_t id) {
	if (list == NULL)
		return 1;
 
	list->id = id;
	list->nodeQtd = 0;
	list->head = NULL;
	return 0;
}

int32_t criarNode(struct Node ** node, uint32_t data, uint32_t id) {
	struct Node * newNode = malloc(sizeof(struct Node));
	if (newNode == NULL)
		return 1;

	newNode->id = id;
	newNode->data = data;
	newNode->next = NULL;

	*node = newNode;
	return 0;
}

int32_t insereListaFim(struct List * list, struct Node ** node) {
	if (list == NULL)
		return 1;

	if (list->head == NULL) {
		list->head = * node;
	} else {
		struct Node * temp = list->head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = * node;
	}

	list->nodeQtd++;
	
	return 0;
}

int32_t removeListaFim(struct List * list, struct Node ** ret) {
	if (list == NULL || list->head == NULL)
		return 1;

	struct Node * temp1 = list->head;
	struct Node * temp2 = temp1->next;

	if (temp2 == NULL) {
		(*ret)->data = temp1->data;
		(*ret)->id = temp1->id;
		list->head = NULL;
		free(temp1);
		list->nodeQtd--;
		return 0;
	}

	while (temp2->next != NULL) {
		temp1 = temp1->next;
		temp2 = temp2->next;
	}

	(*ret)->data = temp2->data;
	(*ret)->id = temp2->id;

	temp1->next = NULL;
	free(temp2);
	list->nodeQtd--;

	return 0;
}

int32_t buscaLista(struct List * list, uint32_t id, struct Node ** ret) {
	if (list == NULL)
		return 1;

	struct Node * temp = list->head;
	while (temp != NULL) {
		if (temp->id == id) {
			*ret = temp;
			return 0;
		}
		temp = temp->next;
	}
	return 1;
}

void liberarLista(struct List * list) {
	struct Node * atual = list->head;
	while (atual != NULL) {
		struct Node * prox = atual->next;
		free(atual);
		atual = prox;
	}
	list->head = NULL;
	list->nodeQtd = 0;
}

void imprimirLista(struct List * list) {
	struct Node * atual = list->head;
	while (atual != NULL) {
		printf("%d ", atual->data);
		atual = atual->next;
	}
}
