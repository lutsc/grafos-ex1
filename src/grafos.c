#include "grafos.h"

int32_t iniciarGrafo(struct Graph * graph, uint32_t vertices, bool dirigido) {
	if (graph == NULL) {
		// Grafo inválido
		return 1;
	}
	
	if (vertices <= 0) {
		// Argumentos inválidos
		return 1;
	}

	graph->verticesQtd = vertices;
	graph->directed = dirigido;
 
	graph->array = (struct List *)calloc(graph->verticesQtd, sizeof(struct List));
	if (graph->array == NULL) {
		printf("\nErro ao alocar memória para o vetor de listas.\n");
		return 1;
	}
 
	for (uint32_t i = 0; i < graph->verticesQtd; i++) {
		iniciarLista(&graph->array[i], (i + 1));
	}
 
	return 0;
}

int32_t liberaGrafo(struct Graph * graph) {
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        liberarLista(&graph->array[i]);
	}

    free(graph->array);
    graph->array = NULL;
	graph->verticesQtd = 0;
	return 0;
}

/*
 * Checa se há conexão já existente, se não, adiciona conexão para id2 no vértice id1, idem se o grafo é dirigido
 */
int32_t inserirAresta(struct Graph * graph, uint32_t id1, uint32_t id2){
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

	if (id1 == id2 || id1 <= 0 || id2 <= 0 || id1 > graph->verticesQtd || id2 > graph->verticesQtd) {
		// Vértice inválido
		return 1;
	}

	// Checa se conexão já existe
	struct Node * temp = graph->array[id1-1].head;
	while (temp != NULL) {
		if (temp->id == id2)
			return 1;
		temp = temp->next;
	}

	// Conexão id1 -> id2
	struct Node * novoNode;
	if (criarNode(&novoNode, 0, id2))
		return 1;
	insereListaFim(&graph->array[id1-1], &novoNode);

	// Se não dirigido
	// Conexão id2 -> id1
	if (graph->directed == 0) {
		struct Node * novoNode2;
		if (criarNode(&novoNode2, 0, id1))
			return 1;
		insereListaFim(&graph->array[id2-1], &novoNode2);
	}

	return 0;
}

int32_t removerAresta(struct Graph * graph, uint32_t id1, uint32_t id2) {
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

	if (id1 == id2 || id1 <= 0 || id2 <= 0 || id1 > graph->verticesQtd || id2 > graph->verticesQtd) {
		// Vértice inválido
		return 1;
	}

	struct List * lista = &graph->array[id1 - 1];
    struct Node * atual = lista->head;
    struct Node * anterior = NULL;
    
	// Percorrendo para achar a conexão dada
    while (atual != NULL && atual->id != id2) {
        anterior = atual;
        atual = atual->next;
    }
    
    if (atual == NULL) {
		// Aresta inválida
        return 1;
    }
    
	// Remove conexão e conecta as outras conexões do lado
    if (anterior == NULL) {
        lista->head = atual->next;
    } else {
        anterior->next = atual->next;
    }
    free(atual);
    lista->nodeQtd--;
    
	// Se não dirigido, faz a mesma coisa com o outro node dado
    if (graph->directed == 0) {
        struct List * lista2 = &graph->array[id2 - 1];
        atual = lista2->head;
        anterior = NULL;

		// Percorrendo para achar a conexão dada
        while (atual != NULL && atual->id != id1) {
            anterior = atual;
            atual = atual->next;
        }

		// Já que não é dirigido, assume que existe a conexão em sua contraparte
        if (atual != NULL) {
			// Remove conexão e conecta as outras conexões do lado
            if (anterior == NULL) {
                lista2->head = atual->next;
            } else {
                anterior->next = atual->next;
            }
            free(atual);
            lista2->nodeQtd--;
        }
    }
    
    return 0;
}

/*
 * Insere nova vertice no grafo, realocando o array de listas encadeadas com mais um elemento
 */
int32_t inserirVertice(struct Graph * graph){
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

	uint32_t novoId = graph->verticesQtd;

	struct List * novoArray = realloc(graph->array, (novoId + 1) * sizeof(struct List));
	if (novoArray == NULL)
		return 1;
	
	graph->array = novoArray;
	iniciarLista(&graph->array[novoId], (novoId + 1));
	graph->verticesQtd++;

	return 0;
}

int32_t removerVertice(struct Graph * graph, uint32_t id) {
	if (graph == NULL || id == 0 || id > graph->verticesQtd) {
		// Grafo ou Vértice inválido
		return 1;
	}

    if (graph->verticesQtd <= 1) {
		// Temporário, impede de deixar o grafo com 0 vértices
        return 1;
    }
    
    // Remove todas as conexões para o vértice dado
    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        if (i != (id - 1)) {
            removerAresta(graph, (i + 1), id);
        }
    }
    
    // Libera a lista do vértice
    liberarLista(&graph->array[id - 1]);
    
    // Percorre a partir do vértice dado
	// Desloca os vértices para cobrir o espaço removido
    for (uint32_t i = (id - 1); i < (graph->verticesQtd - 1); i++) {
        graph->array[i] = graph->array[(i + 1)];
        graph->array[i].id = (i + 1);
    }
    graph->verticesQtd--;
    
    // Atualiza os id's nos nós
    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        struct Node * atual = graph->array[i].head;
        while (atual != NULL) {
            if (atual->id > id) {
                atual->id--;
            }
            atual = atual->next;
        }
    }
    
    return 0;
}

int32_t gerarMatrizAdjacente(struct Graph * graph, bool ***mat) {
	*mat = malloc(sizeof(bool*) * graph->verticesQtd);

	struct Node * atual;

	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		atual  = graph->array[i].head;
		(*mat)[i] = calloc(graph->verticesQtd, sizeof(bool));
		while(atual != NULL)
		{
			(*mat)[i][atual->id-1] = 1;
			atual = atual->next;
		}
	}
	return 0;
}
