#include "menu.h"

uint32_t module(Vector2 vec)
{
	return sqrt(pow(vec.x,2)+pow(vec.y,2));
}

Vector2 concentricPointStop(Vector2 a, Vector2 b, float r) {
	double D = (double)module((Vector2){b.x - a.x, b.y - a.y});
	// double D = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));

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
	wing1.x = bX * cos(angle) - bY * sin(angle) + pos2.x;
	wing1.y = bX * sin(angle) + bY * cos(angle) + pos2.y;

	Vector2 wing2;
	wing2.x = bX * cos(-angle) - bY * sin(-angle) + pos2.x;
	wing2.y = bX * sin(-angle) + bY * cos(-angle) + pos2.y;

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
	DrawLine(rect.x, rect.y+rect.height, rect.x+rect.width, rect.y, lineColor);
	
	return clicked;
}

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

int32_t TextButtonToggle(Rectangle rect, const char * label, bool enabled)
{
	bool clicked = false;
	Color buttonColor = BUTTON_COLOR;

	if(!enabled) {
		buttonColor = GRAY;
	}

	if(CheckCollisionPointRec(GetMousePosition(), rect)) {
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

void drawNumberMiddleLine(Vector2 pos1, Vector2 pos2, uint32_t number) {
	Vector2 final = pos1;
	final.x += (pos2.x - pos1.x)/2;
	final.y += (pos2.y - pos1.y)/2 - 3*(float)FONT_SIZE;

	// Vector2 hyp = {fabs(final.x - pos1.x), fabs(final.y - pos1.y)};

	// double angle = acos(module(final));
	// double angle = 90;
	// DrawTextPro(GetFontDefault(), TextFormat("%d", number), final, final, angle, (float)FONT_SIZE, 1, BLACK);
	DrawText(TextFormat("%d", number), final.x, final.y, FONT_SIZE, BLACK);
}