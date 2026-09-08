#include <raylib.h>
#include "grafos.h"

#include <raygui.h>
#define RAYGUI_IMPLEMENTATION

#define WIDTH 800
#define HEIGHT 600

#define BAR_HEIGHT (int)(HEIGHT/10)

#define BUTTON_COLOR BLUE
#define CIRCLE_RADIUS 15

#define V_COLOR PURPLE

void mostrarGrafo(struct Graph * graph) {
    if (graph == NULL || graph->array == NULL) {
        printf("Grafo inválido ou vazio.\n");
        return;
    }
    
    printf("\n- LISTA DE ADJACÊNCIA -\n");
    for (uint32_t i = 0; i < graph->verticesQtd; i++) {
        printf("%d -> ", i + 1);
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
            printf(" %d ", i + 1);
        }
        printf("\n");
        
        for (uint32_t i = 0; i < graph->verticesQtd; i++) {
            printf(" %d ", i + 1);
            for (uint32_t j = 0; j < graph->verticesQtd; j++) {
                printf(" %d ", mat[i][j]);
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

enum MOUSE_STATE {
	SELECT, SELECT_CIRCLE
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

	// ToggleFullscreen();

	while(!WindowShouldClose())
	{

		switch(GetKeyPressed())
		{
			case KEY_Q:
				mostrarGrafo(&graph);
				exit(0);
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
				case SELECT_CIRCLE:
					mouseState = SELECT;
					break;

				default:
					break;
			}
		}



		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawRectangle(0, 0, WIDTH, BAR_HEIGHT, LIGHTGRAY);

		if(CircleButton(WIDTH/20, BAR_HEIGHT/2, CIRCLE_RADIUS)) {
			mouseState = SELECT_CIRCLE;
		}

		switch(mouseState)
		{
			case SELECT_CIRCLE:
				DrawCircleV(GetMousePosition(), CIRCLE_RADIUS, ColorAlpha(BUTTON_COLOR, 0.7));
				break;
			default:
				break;
		}

		for(uint32_t i = 0; i < graph.verticesQtd; i++)
		{
			DrawCircleV(graph.array[i].pos, CIRCLE_RADIUS, V_COLOR);
			if(CheckCollisionPointCircle(GetMousePosition(), graph.array[i].pos, CIRCLE_RADIUS))
			{
				DrawCircleLinesV(graph.array[i].pos, CIRCLE_RADIUS, BLACK);

				if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)) {
					removerVertice(&graph, graph.array[i].id); //NOTE: Mover para um menu de contexto
				}

			}
		}

		EndDrawing();
	}
}
