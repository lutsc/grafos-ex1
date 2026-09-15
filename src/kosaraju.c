#include "kosaraju.h"

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