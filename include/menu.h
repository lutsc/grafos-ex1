#ifndef H_MENU
#define H_MENU 1

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <raylib.h>
#include <math.h>

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

enum MOUSE_STATE {
	SELECT, SELECT_CIRCLE, SELECT_LINE
};

uint32_t module(Vector2 vec);

Vector2 concentricPointStop(Vector2 a, Vector2 b, float r);

void DrawArrow(Vector2 pos1, Vector2 pos2, float angle, float wing_size, Color color);

int32_t CircleButton(int32_t posX, int32_t posY, int32_t radius);

int32_t LineButton(Rectangle rect);

/*
 * Botão retangular com texto. Retorna se foi clicado.
 * Se "enabled" for false, aparece cinza e não responde ao clique.
 */
int32_t TextButton(Rectangle rect, const char * label, bool enabled);

/*
 * Similar ao TextButton mas permite ser clicado mesmo inativo
 */
int32_t TextButtonToggle(Rectangle rect, const char * label, bool enabled);

void drawNumberMiddleLine(Vector2 pos1, Vector2 pos2, uint32_t number);

#endif