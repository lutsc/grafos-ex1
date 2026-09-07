#include <stdio.h>
#include <stdlib.h>
#include "lista_encadeada.h"
#include "grafos.h"
#include "matrizes.h"

static void menu(void) {
	printf("\n===== MENU =====\n");
	printf("[1]  Criar novo grafo\n");
	printf("[2]  Inserir vértice\n");
	printf("[3]  Remover vértice\n");
	printf("[4]  Inserir aresta/arco\n");
	printf("[5]  Remover aresta\n");
	printf("[6]  Mostrar grafo (lista e matriz de adjacência)\n");
	printf("[7]  Percorrer em profundidade (DFS)  (em breve)\n");
	printf("[8]  Percorrer em largura (BFS)       (em breve)\n");
	printf("[9]  Fecho transitivo direto de um vértice   (em breve)\n");
	printf("[10] Fecho transitivo inverso de um vértice  (em breve)\n");
	printf("[11] Verificar conexidade / componentes fortemente conexas (em breve)\n");
	printf("[0]  Sair\n");
	printf("Escolha: ");
}

int main(void) {
	struct Graph graph;
	bool existeGrafo = false;
	int opcao;

	do {

		menu();

		if (scanf("%d", &opcao) != 1) {
			printf("Entrada inválida.\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			if (c == EOF)
				break;
			continue;
		}
 
		if (opcao != 1 && opcao != 0 && !existeGrafo) {
			printf("Crie um grafo primeiro (opção 1).\n");
			continue;
		}

		switch (opcao) {
			case 1: {
				if (existeGrafo)
					liberaGrafo(&graph);
 
				uint32_t dirigido, vertices;
				printf("O grafo é dirigido? [1] Sim [0] Não: ");
				if (scanf("%u", &dirigido) != 1 || dirigido > 1) {
					printf("Opção inválida.\n");
					existeGrafo = false;
					break;
				}
 
				printf("Quantidade de vértices: ");
				if (scanf("%u", &vertices) != 1 || vertices == 0) {
					printf("Opção inválida.\n");
					existeGrafo = false;
					break;
				}
 
				if (iniciarGrafo(&graph, vertices, (bool)dirigido) != 0) {
					printf("Erro ao criar o grafo.\n");
					existeGrafo = false;
				} else {
					const char * tipo;
					if (dirigido)
						tipo = "dirigido";
					else
						tipo = "não dirigido";
 
					printf("Grafo criado com %u vértices (%s).\n", vertices, tipo);
					existeGrafo = true;
				}
			}

			case 2: {
				if (inserirVertice(&graph) == 0)
					printf("Vértice %u inserido.\n", graph.verticesQtd);
				else
					printf("Erro ao inserir vértice.\n");
				break;
			}

			case 3: {
				uint32_t v;
				printf("Vértice a remover (1 a %u): ", graph.verticesQtd);
				scanf("%u", &v);
				if (removerVertice(&graph, v) == 0)
				    printf("Vértice %u removido.\n", v);
				else
				    printf("Vértice inválido.\n");
				
				break;
			}

			case 4: {
				uint32_t origem, destino;
				printf("Vértice de origem (1 a %u): ", graph.verticesQtd);
				scanf("%u", &origem);
				printf("Vértice de destino (1 a %u): ", graph.verticesQtd);
				scanf("%u", &destino);

				if (inserirAresta(&graph, origem, destino) == 0)
					printf("Aresta %u -> %u inserida.\n", origem, destino);
				else
					printf("Erro ao inserir aresta (vértice inválido ou já existente).\n");
				break;
			}

			case 5: {
				uint32_t origem, destino;
				printf("Vértice de origem (1 a %u): ", graph.verticesQtd);
				scanf("%u", &origem);
				printf("Vértice de destino (1 a %u): ", graph.verticesQtd);
				scanf("%u", &destino);
				if (removerAresta(&graph, origem, destino) == 0)
				    printf("Aresta %u -> %u removida.\n", origem, destino);
				else
				    printf("Aresta não encontrada ou vértice inválido.\n");
				
				break;
			}

			case 6: {
				// TODO: Necessário a implantação do mostrar grafo.
				// mostrarGrafo(&graph);
				// break;
			}

			case 7: {
				uint32_t v;
				printf("Vértice inicial (1 a %u): ", graph.verticesQtd);
				scanf("%u", &v);
				printf("DFS a partir de %u: ", v);
				if (dfs(&graph, v) != 0)
				    printf("\nVértice inválido.\n");
				
				break;
			}

			case 8: {
				uint32_t v;
				printf("Vértice inicial (1 a %u): ", graph.verticesQtd);
				scanf("%u", &v);
				printf("BFS a partir de %u: ", v);
				if (bfs(&graph, v) != 0)
				    printf("\nVértice inválido.\n");
				
				break;
			}

			case 9: {
				uint32_t v;
				printf("Vértice (1 a %u): ", graph.verticesQtd);
				scanf("%u", &v);
				if (ftd(&graph, v) != 0)
				    printf("Vértice inválido.\n");
				
				break;
			}

			case 10: {
				uint32_t v;
				printf("Vértice (1 a %u): ", graph.verticesQtd);
				scanf("%u", &v);
				if (fdti(&graph, v) != 0)
			    printf("Vértice inválido.\n");
				
				break;
			}

			case 11: {
				if (grafoConexo(&graph)) {
				    printf("O grafo é conexo.\n");
				} else {
				    printf("O grafo NÃO é conexo. Subgrafos fortemente conexos máximos:\n");
				    componentesFortementeConexas(&graph);
				}

				break;
			}

			case 0:
				printf("Encerrando...\n");
				break;

			default:
				printf("Opção inválida.\n");

		}
	} while (opcao != 0);

	if (existeGrafo)
		liberaGrafo(&graph);

	return 0;
}
