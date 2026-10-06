/*
 * Comandos atuais:
 * [q]: Fecha a janela e imprime o grafo no terminal
 * [s]: Imprime o grafo no terminal
 * [clique do meio do mouse]: remove um vértice
 * [clique esquerdo do mouse]: movimenta um vértice ou cria um vértice ou aresta
 * [f]: Fecho transitivo direto do vértice selecionado
 * [i]: Fecho transitivo inverso do vértice selecionado
 * [d]: Percorre em profundidade (DFS) a partir do vértice selecionado
 * [b]: Percorre em largura (BFS) a partir do vértice selecionado
 * [c]: Verifica conexidade / componentes fortemente conexos
 *
 * Os atalhos f, i, d, b e c também têm botão na barra superior (FTD, FTI, DFS, BFS, Conexo).
 * Os de vértice (FTD, FTI, DFS, BFS) ficam cinza até haver um vértice selecionado.
 */

// #include "grafos.h"
#include "menu.h"
#include "fechotransitivo.h"
#include "busca.h"
#include "kosaraju.h"
#include "coloracao.h"

void mostraVetor (int32_t vet[], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", vet[i]);
	}
	puts("");
}

/*
 * Ações sobre o grafo (usadas tanto pelos botões quanto pelo teclado)
 */
void acaoFtd(struct Graph * graph, struct List * currentVertice)
{
	if(currentVertice == NULL)
		return;
	int * ftdA = malloc(graph->verticesQtd * sizeof(int));
	ftdGrafo(graph, currentVertice->id - 1, ftdA);
	printf("Ftd do vértice %d:", currentVertice->id);
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		printf("%d ", ftdA[i]);
	}
	puts("");
}

void acaoFti(struct Graph * graph, struct List * currentVertice)
{
	if(currentVertice == NULL)
		return;
	int * ftiA = malloc(graph->verticesQtd * sizeof(int));
	ftdiGrafo(graph, currentVertice->id - 1, ftiA);
	printf("Fti do vértice %d:", currentVertice->id);
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		printf("%d ", ftiA[i]);
	}
	puts("");
}

void acaoDfs(struct Graph * graph, struct List * currentVertice)
{
	if(currentVertice == NULL)
		return;
	if(dfs(graph, currentVertice->id) != 0)
		printf("\nVértice inválido.\n");
	puts("");
}

void acaoBfs(struct Graph * graph, struct List * currentVertice)
{
	if(currentVertice == NULL)
		return;
	if(bfs(graph, currentVertice->id) != 0)
		printf("\nVértice inválido.\n");
	puts("");
}

void acaoConexidade(struct Graph * graph)
{
	if(graph->verticesQtd == 0)
		return;
	if(grafoConexo(graph)) {
		printf("O grafo é conexo.\n");
	} else {
		printf("O grafo NÃO é conexo.\n\n");
		printf("Subgrafos fortemente conexos máximos:\n");
		componentesFortementeConexos(graph);
	}
}

void acaoDijkstra(struct Graph * graph, struct List * currentVertice, int32_t est[], int32_t prev[])
{
	if(currentVertice == NULL)
		return;

	est = malloc(sizeof(int32_t)*graph->verticesQtd);
	prev = malloc(sizeof(int32_t)*graph->verticesQtd);

	if(!dijkstra(graph, currentVertice->id - 1, est, prev))
	{
		puts("Dijkstra: \n");
		printf("Estimado: ");mostraVetor(est, graph->verticesQtd);
		printf("Antecessor: ");mostraVetor(prev, graph->verticesQtd);
	}
	free(est);
	free(prev);
}

void calcularCores(struct Graph * graph)
{
	int32_t cores[graph->verticesQtd];

	verticesColoracaoGrafo(graph, cores);
	atualizarColoracaoGrafo(graph, cores);
}

/*
 * Retorna uma cor dado um número inteiro positivo
 */
Color getColor(uint32_t num){ 
	uint8_t r = 255 * (bool)(num&0b00000100);
	uint8_t g = 255 * (bool)(num&0b00000010);
	uint8_t b = 255 * (bool)(num&0b00000001);
	for(int i = 0; i < 5; i++){
		r -= (128 >> i) * (bool)(num&(0b00001000 << i) * (bool)(num&0b00000100)) + (255>>i)*(num&0b11111000);
		g -= (128 >> i) * (bool)(num&(0b00001000 << i) * (bool)(num&0b00000010)) + (255>>i)*(num&0b11111000);
		b -= (128 >> i) * (bool)(num&(0b00001000 << i) * (bool)(num&0b00000001)) + (255>>i)*(num&0b11111000);
	}

	return (Color){r, g, b, 255};
}

int main()
{
	SetTraceLogLevel(LOG_NONE);

	InitWindow(WIDTH, HEIGHT, "Grafos");
	SetTargetFPS(60);

	enum MOUSE_STATE mouseState = SELECT;

	SetExitKey(KEY_NULL);

	struct Graph graph;
	graph.verticesQtd = 0; 
	graph.directed = 1;

	iniciarGrafo(&graph, graph.verticesQtd, graph.directed);

	struct List * currentVertice = NULL;
	struct List * hoverVertice = NULL;
	struct List * remover1 = NULL;
	struct List * remover2 = NULL;
	struct Node * tempNode = NULL;

	int32_t * prev = malloc(sizeof(int32_t));
	int32_t * est = malloc(sizeof(int32_t));

	bool verticeCollision = false;

	Vector2 mousePos;

	bool mostrarCores = false;
	bool isSelected = (currentVertice != NULL);
	bool atualizarGrafo = false;


	float buttonW = 55; 
	float buttonH = 30;
	float buttonGap = 10;
	float buttonX = (int)(WIDTH/20) + PADDING + 50;
	float buttonY = (int)(BAR_HEIGHT/2) - buttonH/2;

	// ToggleFullscreen();

	while(!WindowShouldClose())
	{
		mousePos = GetMousePosition();

		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawRectangle(0, 0, WIDTH, BAR_HEIGHT, LIGHTGRAY);

		if(CircleButton(WIDTH/20, BAR_HEIGHT/2, CIRCLE_RADIUS)) {
			mouseState = SELECT_CIRCLE;
		}
		else if(LineButton((Rectangle){(int)(WIDTH/20+PADDING), (int)(BAR_HEIGHT/2)-15, 15, 30})) {
			mouseState = SELECT_LINE;
		}

		isSelected = (currentVertice != NULL);
		buttonX = (int)(WIDTH/20) + PADDING + 50; 
		if(TextButton((Rectangle){buttonX, buttonY, buttonW, buttonH}, "FTD", isSelected))
			acaoFtd(&graph, currentVertice);
		buttonX += buttonW + buttonGap;

		if(TextButton((Rectangle){buttonX, buttonY, buttonW, buttonH}, "FTI", isSelected))
			acaoFti(&graph, currentVertice);
		buttonX += buttonW + buttonGap;

		if(TextButton((Rectangle){buttonX, buttonY, buttonW, buttonH}, "DFS", isSelected))
			acaoDfs(&graph, currentVertice);
		buttonX += buttonW + buttonGap;
	
		if(TextButton((Rectangle){buttonX, buttonY, buttonW, buttonH}, "BFS", isSelected))
			acaoBfs(&graph, currentVertice);
		buttonX += buttonW + buttonGap;

		if(TextButton((Rectangle){buttonX, buttonY, buttonW, buttonH}, "Dijkstra", isSelected))
			acaoDijkstra(&graph, currentVertice, est, prev);
		buttonX += buttonW + buttonGap;

		if(TextButton((Rectangle){buttonX, buttonY, buttonW, buttonH}, "Conexo", graph.verticesQtd > 0))
			acaoConexidade(&graph);
		buttonX += buttonW + buttonGap;

		if(TextButtonToggle((Rectangle){buttonX, buttonY, buttonW, buttonH}, "Cor", graph.verticesQtd > 0 && mostrarCores)) {
			mostrarCores = !mostrarCores;
		}

		switch(mouseState)
		{
			case SELECT_CIRCLE:
				// DrawCircleV(GetMousePosition(), CIRCLE_RADIUS, ColorAlpha(BUTTON_COLOR, 0.7));
				DrawPoly(mousePos,CIRCLE_RESOLUTION, CIRCLE_RADIUS, 0, ColorAlpha(BUTTON_COLOR, 0.7));
				break;
			case SELECT_LINE:
				DrawLine(mousePos.x-15, mousePos.y+15,mousePos.x+15, mousePos.y-15, ColorAlpha(BLACK, 0.7));
				break;
			default:
				break;
		}

		verticeCollision = false;
		hoverVertice = NULL;
		for(uint32_t i = 0; i < graph.verticesQtd; i++)
		{
			// DrawCircleV(graph.array[i].pos, CIRCLE_RADIUS, V_COLOR);
			if(!mostrarCores) {
				DrawPoly(graph.array[i].pos, CIRCLE_RESOLUTION, CIRCLE_RADIUS, 0, V_COLOR);
			}
			else{
				DrawPoly(graph.array[i].pos, CIRCLE_RESOLUTION, CIRCLE_RADIUS, 0, getColor(graph.array[i].color)); 
			}
			DrawText(TextFormat("%d", graph.array[i].id), graph.array[i].pos.x-(int)(MeasureText(TextFormat("%d", graph.array[i].id), FONT_SIZE)/2), graph.array[i].pos.y-(int)(FONT_SIZE), FONT_SIZE, BLACK);
			tempNode = graph.array[i].head;
			while(tempNode != NULL)
			{

				if(CheckCollisionCircleLine(mousePos, 15, graph.array[i].pos, graph.array[tempNode->id-1].pos))
				{
					DrawArrow(concentricPointStop(graph.array[tempNode->id-1].pos, graph.array[i].pos, CIRCLE_RADIUS), concentricPointStop(graph.array[i].pos, graph.array[tempNode->id-1].pos, CIRCLE_RADIUS), ARROW_ANGLE, ARROW_WING, RED);
					drawNumberMiddleLine(graph.array[i].pos, graph.array[tempNode->id-1].pos, tempNode->data);
					if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
						remover1 = &graph.array[i];
						remover2 = &graph.array[tempNode->id-1];
					}
				}
				else {
					DrawArrow(concentricPointStop(graph.array[tempNode->id-1].pos, graph.array[i].pos, CIRCLE_RADIUS), concentricPointStop(graph.array[i].pos, graph.array[tempNode->id-1].pos, CIRCLE_RADIUS), ARROW_ANGLE, ARROW_WING, BLACK);
					// drawNumberMiddleLine((Vector2){300, 300}, (Vector2){400, 300}, 10);
					drawNumberMiddleLine(graph.array[i].pos, graph.array[tempNode->id-1].pos, tempNode->data);
				}


				// DrawLineV(graph.array[i].pos, graph.array[tempNode->id-1].pos, BLACK);


				tempNode = tempNode->next;
			}
			if(CheckCollisionPointCircle(mousePos, graph.array[i].pos, CIRCLE_RADIUS))
			{
				hoverVertice = &graph.array[i];
				verticeCollision = true;
				
				if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
					switch(mouseState){
						case SELECT:
							// if(currentVertice == NULL)
							// {
							// 	currentVertice = &graph.array[i];
							// }
							currentVertice = &graph.array[i];
							break;

						case SELECT_LINE:
							if(currentVertice == NULL)
							{
								currentVertice = &graph.array[i];
							}
							else{
								inserirAresta(&graph, currentVertice->id, graph.array[i].id, 1);
								currentVertice = &graph.array[i];
								atualizarGrafo = true;
							}
						default:
							break;

						}
				}

				else if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
					removerVertice(&graph, graph.array[i].id);
					if(currentVertice == &graph.array[i])
						currentVertice = NULL;

					atualizarGrafo = true;
				}

			}
		}
		if(currentVertice != NULL)
		{
			// DrawCircleLinesV(currentVertice->pos, CIRCLE_RADIUS, RED);
			DrawPolyLines(currentVertice->pos, CIRCLE_RESOLUTION, CIRCLE_RADIUS, 0, RED);
		}
		if(hoverVertice != NULL && hoverVertice != currentVertice)
		{
			// DrawCircleLinesV(graph.array[i].pos, CIRCLE_RADIUS, BLACK);
			DrawPolyLines(hoverVertice->pos, CIRCLE_RESOLUTION, CIRCLE_RADIUS, 0, BLACK);
		}

		if(remover1 != NULL && remover2 != NULL)
		{
			removerAresta(&graph, remover1->id, remover2->id);
			remover1 = NULL;
			remover2 = NULL;
			atualizarGrafo = true;
		}

		if(atualizarGrafo){
			calcularCores(&graph);
			atualizarGrafo = false;
		}

		switch(GetKeyPressed())
		{
			case KEY_Q:
				mostrarGrafo(&graph);
				exit(0);
			case KEY_S:
				mostrarGrafo(&graph);
				break;
			case KEY_F:
				acaoFtd(&graph, currentVertice);
				break;
			case KEY_I:
				acaoFti(&graph, currentVertice);
				break;
			case KEY_D:
				acaoDfs(&graph, currentVertice);
				break;
			case KEY_B:
				acaoBfs(&graph, currentVertice);
				break;
			case KEY_C:
				acaoConexidade(&graph);
				break;
			case KEY_V:
			case KEY_ONE:
				mouseState = SELECT_CIRCLE;
				break;
			case KEY_A:
			case KEY_TWO:
				mouseState = SELECT_LINE;
				break;
			case KEY_SPACE:
			case KEY_ESCAPE:
				mouseState = SELECT;
				break;
			case KEY_BACKSPACE:
			case KEY_DELETE:
				if (currentVertice != NULL) {
					removerVertice(&graph, currentVertice->id);
					currentVertice = NULL;
					atualizarGrafo = true;
				}
				break;
			default:
				break;
		}

		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			switch(mouseState)
			{
				case SELECT:
					if(!verticeCollision && mousePos.y > BAR_HEIGHT)
						currentVertice = NULL;
					break;
				case SELECT_CIRCLE:
					if(mousePos.y > BAR_HEIGHT+CIRCLE_RADIUS) {
						inserirVertice(&graph);
						atualizarGrafo = true;
					}
					break;
				case SELECT_LINE:
					if(!verticeCollision && mousePos.y > BAR_HEIGHT)
						currentVertice = NULL;
					break;

				default:
					break;
			}
		}
		else if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
		{
			switch(mouseState)
			{
				default:
					currentVertice = NULL;
					mouseState = SELECT;
					break;
			}
		}

		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			if(currentVertice != NULL && mouseState == SELECT && mousePos.y > BAR_HEIGHT+CIRCLE_RADIUS && module(GetMouseDelta()) > 0)
			{
				currentVertice->pos = mousePos;
			}
		}
		EndDrawing();
	}
	return 0;
}
