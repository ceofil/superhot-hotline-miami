#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

class Soldier
{
public:
	Soldier() = default;
	Soldier(Vec2 pos, Vec2 dir);
	void Update(Keyboard& kbd, Mouse& mouse, float dt);
	void Draw(Graphics& gfx);
public:
	
private:
	Vec2 pos;
	Vec2 dir;
	static constexpr float speed = 0.5f;
	static constexpr float radius = 15.0f;
};

