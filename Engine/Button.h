#pragma once
#include "RectF.h"
#include "Graphics.h"
#include "Mouse.h"
#include "Text.h"

class Button
{
public:
	Button() = default;
	Button(RectF rect_in,const char* text_in, int pixelSize_in, Color color_in);
	void Draw(Graphics& gfx, Mouse& mouse, Text& txt);
	bool IsMouseOver(Mouse& mouse);
private:
	RectF rect;
	Color color;
	Color mouseOverColor;
	int pixelSize;
	char text[15];
};