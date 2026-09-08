/*
 * Comandos atuais:
 * [q]: Fecha a janela e imprime o grafo no terminal
 * [s]: Imprime o grafo no terminal
 * [clique do meio do mouse]: remove um vértice
 * [clique esquerdo do mouse]: movimenta um vértice ou cria um vértice ou aresta
 */

#include <raylib.h>
#include "grafos.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define WIDTH 800
#define HEIGHT 600

#define BAR_HEIGHT (int)(HEIGHT/10)

#define BUTTON_COLOR BLUE
#define CIRCLE_RADIUS 15
#define PADDING 60

#define FONT_SIZE CIRCLE_RADIUS/5

#define V_COLOR PURPLE

void mostrarGrafo(struct Graph * graph) {
    if (graph == NULL || graph->array == NULL) {
        printf("Grafo inválido ou vazio.\n");
        return;
    }
    
    printf("\n- LISTA DE ADJACÊNCIA -\n");
    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        printf("%2d -> ", i + 1);
        struct List * atual = &graph->array[i];
        if (atual == NULL) {
            printf("NULL");
        }
		imprimirLista(atual);
        printf("\n");
    }
    
    printf("\n- MATRIZ DE ADJACÊNCIA -\n");
    bool ** mat = NULL;
    if (gerarMatrizAdjacente(graph, &mat) == 0) {
        printf("   ");
        for (uint32_t i = 0; i < graph->verticesQtd; i++) {
            printf(" %2d ", i + 1);
        }
        printf("\n");
        
        for (uint32_t i = 0; i < graph->verticesQtd; i++) {
            printf(" %d ", i + 1);
            for (uint32_t j = 0; j < graph->verticesQtd; j++) {
                printf(" %2d ", mat[i][j]);
            }
            printf("\n");
        }
        
        liberarMatriz(mat, graph->verticesQtd);
    }
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
DrawCircle(posX, posY, radius, circleColor); return clicked;
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
	graph.directed = 0;

	iniciarGrafo(&graph, graph.verticesQtd, graph.directed);

	struct List * currentVertice = NULL;
	struct Node * tempNode = NULL;

	// ToggleFullscreen();

	while(!WindowShouldClose())
	{

		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawRectangle(0, 0, WIDTH, BAR_HEIGHT, LIGHTGRAY);

		if(CircleButton(WIDTH/20, BAR_HEIGHT/2, CIRCLE_RADIUS)) {
			mouseState = SELECT_CIRCLE;
		}
		else if(LineButton((Rectangle){(int)(WIDTH/20+PADDING), (int)(BAR_HEIGHT/2)-15, 15, 30})) {
			mouseState = SELECT_LINE;
		}

		switch(mouseState)
		{
			case SELECT_CIRCLE:
				DrawCircleV(GetMousePosition(), CIRCLE_RADIUS, ColorAlpha(BUTTON_COLOR, 0.7));
				break;
			case SELECT_LINE:
				DrawLine(GetMousePosition().x-15, GetMousePosition().y+15,GetMousePosition().x+15, GetMousePosition().y-15, ColorAlpha(BLACK, 0.7));
				break;
			default:
				break;
		}

		for(uint32_t i = 0; i < graph.verticesQtd; i++)
		{
			DrawCircleV(graph.array[i].pos, CIRCLE_RADIUS, V_COLOR);
			DrawText(TextFormat("%d", graph.array[i].id), graph.array[i].pos.x-(int)(MeasureText(TextFormat("%d", graph.array[i].id), FONT_SIZE)/2), graph.array[i].pos.y-(int)(FONT_SIZE), FONT_SIZE, BLACK);
			tempNode = graph.array[i].head;
			while(tempNode != NULL)
			{
				DrawLineV(graph.array[i].pos, graph.array[tempNode->id-1].pos, BLACK);
				tempNode = tempNode->next;
			}
			if(CheckCollisionPointCircle(GetMousePosition(), graph.array[i].pos, CIRCLE_RADIUS))
			{
				DrawCircleLinesV(graph.array[i].pos, CIRCLE_RADIUS, BLACK);
				
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
								inserirAresta(&graph, currentVertice->id, graph.array[i].id);
								currentVertice = NULL;
							}
						default:
							break;

						}
				}

				else if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
					removerVertice(&graph, graph.array[i].id); //NOTE: Mover para um menu de contexto (eventualmente eu espero)
				}

			}
		}
		if(currentVertice != NULL)
		{
			DrawCircleLinesV(currentVertice->pos, CIRCLE_RADIUS, RED);
		}

		switch(GetKeyPressed())
		{
			case KEY_Q:
				mostrarGrafo(&graph);
				exit(0);
			case KEY_S:
				mostrarGrafo(&graph);
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
				case SELECT_CIRCLE:
					if(GetMousePosition().y > BAR_HEIGHT+CIRCLE_RADIUS) {
						inserirVertice(&graph);
					}
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
			if(currentVertice != NULL && mouseState == SELECT && GetMousePosition().y > BAR_HEIGHT+CIRCLE_RADIUS)
			{
				currentVertice->pos = GetMousePosition();
			}
		}
		EndDrawing();
	}
}
