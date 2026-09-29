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

#include <raylib.h>
#include <stdlib.h>

#include "grafos.h"
#include "fechotransitivo.h"
#include "busca.h"
#include "kosaraju.h"
#include "dijkstra.h"

#define WIDTH 800
#define HEIGHT 600

#define BAR_HEIGHT (int)(HEIGHT/10)

#define BUTTON_COLOR BLUE
#define CIRCLE_RADIUS 15
#define PADDING 60

#define CIRCLE_RESOLUTION 100

#define ARROW_ANGLE PI/6
#define ARROW_WING 20

#define FONT_SIZE CIRCLE_RADIUS/5

#define V_COLOR PURPLE

#include <stdio.h>
#include <math.h>

void mostraVetor (int32_t vet[], int n);

uint32_t module(Vector2 vec)
{
	return sqrt(pow(vec.x,2)+pow(vec.y,2));
}

Vector2 concentricPointStop(Vector2 a, Vector2 b, float r) {
	double D = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));

	Vector2 c;
	if (D == 0.0) {
		c.x = 0;
		c.y = 0;
		return c;
	}

	if (D <= r)
	  c = a;

	double d = D - r;
	double t = d / D;

	c.x = a.x + t * (b.x - a.x);
	c.y = a.y + t * (b.y - a.y);

	return c;
}

void DrawArrow(Vector2 pos1, Vector2 pos2, float angle, float wing_size, Color color)
{
	float deltaX = pos2.x - pos1.x;
	float deltaY = pos2.y - pos1.y;
	float Length = sqrt(pow(deltaX,2) + pow(deltaY,2));;

	if(Length == 0)
		return;

	float uX = -deltaX/Length;
	float uY = -deltaY/Length;

	double bX = uX * wing_size; 
	double bY = uY * wing_size; 

	Vector2 wing1;
	wing1.x = bX * cos(angle)- bY * sin(angle) + pos2.x;
	wing1.y = bX * sin(angle)+ bY * cos(angle) + pos2.y;

	Vector2 wing2;
	wing2.x = bX * cos(-angle)- bY * sin(-angle) + pos2.x;
	wing2.y = bX * sin(-angle)+ bY * cos(-angle) + pos2.y;

	DrawLineV(pos1, pos2, color);
	DrawLineV(pos2, wing1, color);
	DrawLineV(pos2, wing2, color);
}

/*
 * Returns a boolean representing if the button is pressed or not
 */
int32_t CircleButton(int32_t posX, int32_t posY, int32_t radius)
{
	bool clicked = false;
	Color circleColor = BUTTON_COLOR;

	if(CheckCollisionPointCircle(GetMousePosition(), (Vector2){posX, posY}, radius)) {
		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			clicked = true;
		}
		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			circleColor = ColorBrightness(BUTTON_COLOR, -0.2);
		}
	}
// DrawCircle(posX, posY, radius, circleColor); return clicked;
DrawPoly((Vector2){posX, posY}, CIRCLE_RESOLUTION, radius, 0,circleColor); return clicked;
}

int32_t LineButton(Rectangle rect)
{
	bool clicked = false;
	Color lineColor = BLACK;

	if(CheckCollisionPointRec(GetMousePosition(), rect)) {
		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			clicked = true;
		}
		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			lineColor = ColorBrightness(BUTTON_COLOR, 0.2);
		}
	}
DrawLine(rect.x, rect.y+rect.height, rect.x+rect.width, rect.y, lineColor); return clicked;
}

/*
 * Botão retangular com texto. Retorna se foi clicado.
 * Se "enabled" for false, aparece cinza e não responde ao clique.
 */
int32_t TextButton(Rectangle rect, const char * label, bool enabled)
{
	bool clicked = false;
	Color buttonColor = BUTTON_COLOR;

	if(!enabled) {
		buttonColor = GRAY;
	}
	else if(CheckCollisionPointRec(GetMousePosition(), rect)) {
		if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			clicked = true;
		}
		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			buttonColor = ColorBrightness(BUTTON_COLOR, -0.2);
		}
	}

	DrawRectangleRec(rect, buttonColor);
	int textSize = 14;
	int textW = MeasureText(label, textSize);
	DrawText(label, rect.x + (rect.width - textW)/2, rect.y + (rect.height - textSize)/2, textSize, WHITE);
	return clicked;
}

/*
 * Ações sobre o grafo (usadas tanto pelos botões quanto pelo teclado)
 */
void acaoFtd(struct Graph * graph, struct List * v)
{
	if(v == NULL)
		return;
	int ftdA[graph->verticesQtd] = {};
	ftdGrafo(graph, v->id-1, ftdA);
	printf("Ftd do vértice %d:", v->id);
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		printf("%d ", ftdA[i]);
	}
	puts("");
}

void acaoFti(struct Graph * graph, struct List * v)
{
	if(v == NULL)
		return;
	int ftiA[graph->verticesQtd] = {};
	ftdiGrafo(graph, v->id-1, ftiA);
	printf("Fti do vértice %d:", v->id);
	for(uint32_t i = 0; i < graph->verticesQtd; i++) {
		printf("%d ", ftiA[i]);
	}
	puts("");
}

void acaoDfs(struct Graph * graph, struct List * v)
{
	if(v == NULL)
		return;
	if(dfs(graph, v->id) != 0)
		printf("\nVértice inválido.\n");
	puts("");
}

void acaoBfs(struct Graph * graph, struct List * v)
{
	if(v == NULL)
		return;
	if(bfs(graph, v->id) != 0)
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


void acaoDijkstra(struct Graph * graph, struct List * v, int32_t est[], int32_t prev[])
{
	if(v == NULL)
		return;

	est = malloc(sizeof(int32_t)*graph->verticesQtd);
	prev = malloc(sizeof(int32_t)*graph->verticesQtd);

	if(dijkstra(graph, v->id-1, est, prev))
	{
		puts("Dijkstra: \n");
		printf("Estimado: ");mostraVetor(est, graph->verticesQtd);
		printf("Antecessor: ");mostraVetor(prev, graph->verticesQtd);
	}
	free(est);
	free(prev);
}

enum MOUSE_STATE {
	SELECT, SELECT_CIRCLE, SELECT_LINE
};

int main()
{
	SetTraceLogLevel(LOG_NONE);

	InitWindow(WIDTH, HEIGHT, "Grafos");
	SetTargetFPS(60);

	enum MOUSE_STATE mouseState = SELECT;

	struct Graph graph;
	graph.verticesQtd = 0;
	graph.directed = 1;

	iniciarGrafo(&graph, graph.verticesQtd, graph.directed);

	struct List * currentVertice = NULL;
	struct List * hoverVertice = NULL;
	struct List * remover1 = NULL;
	struct List * remover2 = NULL;
	struct Node * tempNode = NULL;

	int32_t * prev;
	int32_t * est;

	bool verticeCollision = false;

	Vector2 mousePos;

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

		// Botões de ações (as que dependem de vértice ficam cinza sem seleção)
		{
			bool isSelected = (currentVertice != NULL);
			float buttonW = 55;
			float buttonH = 30;
			float buttonGap = 10;
			float buttonX = (int)(WIDTH/20) + PADDING + 50;
			float buttonY = (int)(BAR_HEIGHT/2) - buttonH/2;

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
			if(TextButton((Rectangle){buttonX, buttonY, buttonW + 20, buttonH}, "Conexo", graph.verticesQtd > 0))
				acaoConexidade(&graph);
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
			DrawPoly(graph.array[i].pos, CIRCLE_RESOLUTION, CIRCLE_RADIUS, 0, V_COLOR);
			DrawText(TextFormat("%d", graph.array[i].id), graph.array[i].pos.x-(int)(MeasureText(TextFormat("%d", graph.array[i].id), FONT_SIZE)/2), graph.array[i].pos.y-(int)(FONT_SIZE), FONT_SIZE, BLACK);
			tempNode = graph.array[i].head;
			while(tempNode != NULL)
			{

				if(CheckCollisionCircleLine(mousePos, 15, graph.array[i].pos, graph.array[tempNode->id-1].pos))
				{
					DrawArrow(concentricPointStop(graph.array[tempNode->id-1].pos, graph.array[i].pos, CIRCLE_RADIUS), concentricPointStop(graph.array[i].pos, graph.array[tempNode->id-1].pos, CIRCLE_RADIUS), ARROW_ANGLE, ARROW_WING, RED);
					if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
						remover1 = &graph.array[i];
						remover2 = &graph.array[tempNode->id-1];
					}
				}
				else {
					DrawArrow(concentricPointStop(graph.array[tempNode->id-1].pos, graph.array[i].pos, CIRCLE_RADIUS), concentricPointStop(graph.array[i].pos, graph.array[tempNode->id-1].pos, CIRCLE_RADIUS), ARROW_ANGLE, ARROW_WING, BLACK);
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
								// TODO: Pedir peso para aresta
								inserirAresta(&graph, currentVertice->id, graph.array[i].id, 1);
								currentVertice = &graph.array[i];
							}
						default:
							break;

						}
				}

				else if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
					removerVertice(&graph, graph.array[i].id);
					if(currentVertice == &graph.array[i])
						currentVertice = NULL;
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
				mouseState = SELECT;
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
}

void mostraVetor (int32_t vet[], int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%d ", vet[i]);
	}
	puts("");
}

