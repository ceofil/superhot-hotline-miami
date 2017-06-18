#include "Soldier.h"

Soldier::Soldier(Vec2 pos, Vec2 dir)
	:
	pos(pos),
	dir(dir)
{
}


void Soldier::Update(Keyboard & kbd, Mouse& mouse, float dt)
{
	Vec2 delta = { 0.0f,0.0f };
	if (kbd.KeyIsPressed(0x57))
	{
		delta.y = -1.0f;
	}
	
	else if (kbd.KeyIsPressed(0x53))
	{
		delta.y = 1.0f;
	}
	if (kbd.KeyIsPressed(0x41))
	{
		delta.x = -1.0f;
	}
	else if (kbd.KeyIsPressed(0x44))
	{
		delta.x = 1.0f;
	}

	pos += delta.GetNormalized() * speed;

	dir = (mouse.GetPosVec2() - pos).GetNormalized();
}

void Soldier::Draw(Graphics & gfx)
{
	gfx.DrawCircle(pos, radius, Colors::Green);

	Vec2 aim = pos + dir * radius * 2.5f;
	gfx.DrawCircle(aim, radius*0.25f, Colors::Red);
	gfx.DrawLine(pos, aim, Colors::White);
}
