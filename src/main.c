#include <raylib.h>
#include <raygui.h>
#include "grafos.h"

#define RAYGUI_IMPLEMENTATION

#define WIDTH 800
#define HEIGHT 600

#define BUTTON_COLOR BLUE

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

	DrawCircle(posX, posY, radius, circleColor);
	return clicked;
}

int main()
{
	SetTraceLogLevel(LOG_NONE);


	InitWindow(WIDTH, HEIGHT, "Grafos");
	SetTargetFPS(60);

	// ToggleFullscreen();

	while(!WindowShouldClose())
	{

		switch(GetKeyPressed())
		{
			case KEY_Q:
				exit(0);
			default:
				break;
		}



		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawRectangle(0, 0, WIDTH/5, HEIGHT, LIGHTGRAY);
		CircleButton(WIDTH/10, 40, 20);

		EndDrawing();
	}
}
