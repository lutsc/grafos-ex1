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

int32_t dfs(struct Graph * graph, uint32_t inicio) {
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

	if (inicio <= 0 || inicio > graph->verticesQtd){
		// Vértice inválido
		return 1;
	}

	// Array de vértices visitados
	bool * visitado = (bool *)calloc(graph->verticesQtd, sizeof(bool));
	if (visitado == NULL) {
		free(visitado);
		return 1;
	}

	// Pilha da ordem de visitas
	uint32_t * pilha = malloc(graph->verticesQtd * sizeof(uint32_t));
	if (pilha == NULL) {
		free(visitado);
		return 1;
	}

	int32_t topo = 0;
	pilha[topo] = (inicio - 1);
	printf("\nDFS a partir de %d: ", inicio);

	while (topo >= 0) {
		// Desempilha elemento
		uint32_t atual = pilha[topo--];

		if (!visitado[atual]) {
			visitado[atual] = 1;
			printf("%d ", (atual + 1));

			// Conta os vizinhos do vértice atual
			struct Node * temp = graph->array[atual].head;
			uint32_t vizinhosQtd = 0;
			while (temp != NULL) {
				vizinhosQtd++;
				temp = temp->next;
			}

			// Empilha vizinhos
			if (vizinhosQtd > 0) {
				uint32_t * vizinhos = (uint32_t *)malloc(vizinhosQtd * sizeof(uint32_t));
				if (vizinhos != NULL) {
					temp = graph->array[atual].head;
					for (uint32_t i = 0; i < vizinhosQtd; i++) {
						vizinhos[i] = (temp->id - 1);
						temp = temp->next;
					}

					for (int32_t i = ((int32_t)vizinhosQtd - 1); i >= 0; i--) {
						uint32_t vizinho = vizinhos[i];
						if (!visitado[vizinho]) {
							pilha[++topo] = vizinho;
						}
					}
					free(vizinhos);
				}
			}
		}
	}
	printf("\n");

	free(pilha);
	free(visitado);

	return 0;
}

int32_t bfs(struct Graph * graph, uint32_t inicio) {
	if (graph == NULL || graph->array == NULL) {
		// Grafo ou Lista inválida
		return 1;
	}

	if (inicio <= 0 || inicio > graph->verticesQtd){
		// Vértice inválido
		return 1;
	}

	// Array de vértices visitados
	bool * visitado = (bool *)calloc(graph->verticesQtd, sizeof(bool));
	if (visitado == NULL) {
		free(visitado);
		return 1;
	}

	// Fila da ordem de visitas
	uint32_t * fila = malloc(graph->verticesQtd * sizeof(uint32_t));
	if (fila == NULL) {
		free(fila);
		return 1;
	}

	uint32_t frente = 0, tras = 0;
	visitado[(inicio - 1)] = 1;
	fila[tras++] = (inicio - 1);

	printf("\nBFS a partir de %d: ", inicio);

	while (frente < tras) {
		uint32_t atual = fila[frente++];
		printf("%d ", (atual + 1));

		struct Node * temp = graph->array[atual].head;
		while (temp != NULL) {
			uint32_t vizinho = (temp->id - 1);
			if (!visitado[vizinho]) {
				visitado[vizinho] = 1;
				fila[tras++] = vizinho;
			}
			temp = temp->next;
		}
	}
	printf("\n");

	free(fila);
	free(visitado);

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

/*
 * Retorna o fecho transitivo direto do node passado na função
*/
int32_t ftd(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdA[tamanhoMatriz]) {
	int32_t result[tamanhoMatriz] = {};
	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		result[i] = -1;
	}

	uint32_t current;
	uint32_t * stack = calloc(tamanhoMatriz, sizeof(uint32_t));
	uint32_t top = 0;

	stack[0] = node;
	current = node;
	result[node] = 0;
	bool change;

	while(true)
	{
		change = false;
		for(uint32_t i = 0; i < tamanhoMatriz; i++)
		{
			if(mat[current][i] > 0)
			{
				if(result[i] == -1)
				{
					result[i] = result[current]+1;
					stack[++top] = i; 
					change = true;
				}
			}

		}
		current = stack[top];
		top--;
		if(!change)
			break;
	}
	free(stack);

	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		ftdA[i] = result[i];
	}

	return 0;
}

int32_t ftdGrafo(struct Graph * graph, uint32_t v, int32_t * ftdA) {
	if (graph == NULL || graph->array == NULL || v >= graph->verticesQtd) {
		return 1;
	}

	bool ** mat = NULL;
	gerarMatrizAdjacente(graph, &mat);

	int32_t ret = ftd(mat, graph->verticesQtd, v, ftdA);
	liberarMatriz(mat, graph->verticesQtd);

	return ret;
}

/*
 * Retorna o fecho transitivo direto inverso do node passado na função
*/
int32_t ftdi(bool ** mat, uint32_t tamanhoMatriz, uint32_t node, int32_t ftdiA[tamanhoMatriz]) {
	int32_t result[tamanhoMatriz] = {};
	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		result[i] = -1;
	}

	uint32_t current;
	uint32_t * stack = calloc(tamanhoMatriz, sizeof(uint32_t));
	uint32_t top = 0;

	stack[0] = node;
	current = node;
	result[node] = 0;
	bool change;

	while(true)
	{
		change = false;
		for(uint32_t i = 0; i < tamanhoMatriz; i++)
		{
			if(mat[i][current] > 0)
			{
				if(result[i] == -1)
				{
					result[i] = result[current]+1;
					stack[++top] = i; 
					change = true;
				}
			}

		}
		current = stack[top];
		top--;
		if(!change)
			break;
	}
	free(stack);

	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		ftdiA[i] = result[i];
	}

	return 0;
}

int32_t ftdiGrafo(struct Graph * graph, uint32_t v, int32_t * ftdiA) {
	if (graph == NULL || graph->array == NULL || v >= graph->verticesQtd) {
		return 1;
	}

	bool ** mat = NULL;
	gerarMatrizAdjacente(graph, &mat);

	int32_t ret = ftdi(mat, graph->verticesQtd, v, ftdiA);
	liberarMatriz(mat, graph->verticesQtd);

	return ret;
}

/*
 * Retorna se a o grafo é conexo(1) ou não(0)
*/
int32_t eConexo(bool ** mat, uint32_t tamanhoMatriz)
{
	int32_t ftdA[tamanhoMatriz] = {};
	int32_t ftdiA[tamanhoMatriz] = {};

	ftd(mat, tamanhoMatriz, 0, ftdA);
	ftdi(mat, tamanhoMatriz, 0, ftdiA);

	for(uint32_t i = 0; i < tamanhoMatriz; i++)
	{
		if(ftdA[i] == -1 || ftdiA[i] == -1)
		{
			return 0;
		}
	}
	return 1;
}

int32_t grafoConexo(struct Graph * graph) {
	if (graph == NULL || graph->array == NULL || graph->verticesQtd <= 0) {
		return 1;
	}

	bool ** mat = NULL;
	gerarMatrizAdjacente(graph, &mat);

	int32_t ret = eConexo(mat, graph->verticesQtd);
	liberarMatriz(mat, graph->verticesQtd);
	return ret;
}

/*
 * Função auxiliar para DFS que preenche a pilha em ordem de finalização
 */
static void dfsPreencheOrdem(bool ** mat, uint32_t tam, uint32_t v, bool * visitado, uint32_t * pilha, uint32_t * topo) {
    visitado[v] = true;
    for (uint32_t i = 0; i < tam; i++) {
        if (mat[v][i] && !visitado[i]) {
            dfsPreencheOrdem(mat, tam, i, visitado, pilha, topo);
        }
    }
    pilha[(*topo)++] = v;
}

/*
 * Função auxiliar para DFS que coleta um componente no grafo transposto
 */
static void dfsColetaComponente(bool ** matT, uint32_t tam, uint32_t v, bool * visitado, uint32_t * componente, uint32_t * qtd) {
    visitado[v] = true;
    componente[(*qtd)++] = v;
    for (uint32_t i = 0; i < tam; i++) {
        if (matT[v][i] && !visitado[i]) {
            dfsColetaComponente(matT, tam, i, visitado, componente, qtd);
        }
    }
}

/*
 * Algoritmo de Kosaraju
 * Encontra e imprime os componentes fortemente conexos máximos do grafo
 */
int32_t componentesFortementeConexos(struct Graph * graph) {
    if (graph == NULL || graph->array == NULL) {
        return 1;
    }

    uint32_t tam = graph->verticesQtd;
    if (tam == 0) {
        printf("Grafo vazio.\n");
        return 1;
    }

    bool ** mat = NULL;
    if (gerarMatrizAdjacente(graph, &mat) != 0) {
        printf("Erro ao gerar matriz de adjacência.\n");
        return 1;
    }

    bool * visitado = calloc(tam, sizeof(bool));
    uint32_t * pilha = malloc(tam * sizeof(uint32_t));
    uint32_t topo = 0;

    if (visitado == NULL || pilha == NULL) {
        free(visitado);
        free(pilha);
        liberarMatriz(mat, tam);
        return 1;
    }

    // 1: DFS no grafo original
    for (uint32_t i = 0; i < tam; i++) {
        if (!visitado[i]) {
            dfsPreencheOrdem(mat, tam, i, visitado, pilha, &topo);
        }
    }

    // 2: Grafo transposto
    bool ** matT = malloc(tam * sizeof(bool *));
    if (matT == NULL) {
        free(pilha);
        free(visitado);
        liberarMatriz(mat, tam);
        return 1;
    }
    
    for (uint32_t i = 0; i < tam; i++) {
        matT[i] = calloc(tam, sizeof(bool));
        if (matT[i] == NULL) {
            for (uint32_t j = 0; j < i; j++) {
                free(matT[j]);
            }
            free(matT);
            free(pilha);
            free(visitado);
            liberarMatriz(mat, tam);
            return 1;
        }
    }
    
    for (uint32_t i = 0; i < tam; i++) {
        for (uint32_t j = 0; j < tam; j++) {
            matT[j][i] = mat[i][j];
        }
    }

    // 3: Percorre pilha em ordem decrescente, fazendo DFS no grafo transposto
    for (uint32_t i = 0; i < tam; i++) {
        visitado[i] = false;
    }

    uint32_t * componente = malloc(tam * sizeof(uint32_t));
    if (componente == NULL) {
        free(pilha);
        free(visitado);
        liberarMatriz(mat, tam);
        liberarMatriz(matT, tam);
        return 1;
    }

    uint32_t numComponentes = 0;

    if (!graph->directed) {
        printf("(Grafo não-dirigido - Componentes conexos)\n");
    }

    // Percorrendo pilha em ordem decrescente
    for (int32_t i = (int32_t)tam - 1; i >= 0; i--) {
        uint32_t v = pilha[i];
        if (!visitado[v]) {
            uint32_t qtd = 0;
            dfsColetaComponente(matT, tam, v, visitado, componente, &qtd);
            numComponentes++;
            
            // Ordenar o componente para exibição (bubble sort)
            for (uint32_t a = 0; a < qtd - 1; a++) {
                for (uint32_t b = a + 1; b < qtd; b++) {
                    if (componente[a] > componente[b]) {
                        uint32_t temp = componente[a];
                        componente[a] = componente[b];
                        componente[b] = temp;
                    }
                }
            }
            
            printf("Componente %d: { ", numComponentes);
            for (uint32_t k = 0; k < qtd; k++) {
                printf("%d", componente[k] + 1);
                if (k < qtd - 1) printf(", ");
            }
            printf(" }\n");
        }
    }

    if (numComponentes == 1 && graph->directed) {
        printf("\nO grafo é fortemente conexo!\n");
    } else if (numComponentes == 1 && !graph->directed) {
        printf("\nO grafo é conexo!\n");
    } else {
        printf("\nO grafo possui %d componente%s.\n", numComponentes, numComponentes > 1 ? "s" : "");
    }

    // Liberar memória
    free(componente);
    free(pilha);
    free(visitado);
    liberarMatriz(mat, tam);
    liberarMatriz(matT, tam);

    return 0;
}