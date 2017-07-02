#include "Button.h"

Button::Button(RectF rect_in, const char* text_in, int pixelSize_in, Color color_in)
	:
	rect(rect_in),
	pixelSize(pixelSize_in),
	color(color_in)
{
		strcpy(text,text_in);

		int r = std::min(color.GetR() + 40, 255);
		int g = std::min(color.GetG() + 40, 255);
		int b = std::min(color.GetB() + 40, 255);
		mouseOverColor = Color(r, g, b);
}

void Button::Draw(Graphics & gfx, Mouse & mouse, Text & txt)
{
	if (IsMouseOver(mouse))
	{
		gfx.DrawRectPoints(rect, mouseOverColor);
	}
	else
	{
		gfx.DrawRectPoints(rect, color);
	}


	txt.drawStringSizedCenter(text, int(rect.GetCenter().x), int(rect.GetCenter().y), pixelSize, Colors::White);
}

bool Button::IsMouseOver(Mouse & mouse)
{
	return rect.ContainsPoint( mouse.GetPosVec2() );
}

void Button::SetText(char * source)
{
	strcpy(text, source);
}

RectF Button::GetRect() const
{
	return rect;
}

void Button::SetRect(RectF rect_in)
{
	rect = rect_in;
}
