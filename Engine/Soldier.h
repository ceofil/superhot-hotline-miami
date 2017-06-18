#pragma once
#include "Vec2.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Bullet.h"

class Soldier
{
public:
	Soldier() = default;
	Soldier(Vec2 pos, Vec2 dir);
	void Update(Keyboard& kbd, Mouse& mouse, RectF walls[], int indexWalls, float dt);
	void Draw(Graphics& gfx);
	void Shoot(Bullet bullets[], int nBullets, Sound& bulletShotSound);
	void DoWallCollision(const RectF& wall, Vec2 delta, float dt);

public:
	Vec2 GetBulletSpawnPoint();
	RectF GetRect();
	
private:
	Vec2 pos;
	Vec2 dir;
	static constexpr float speed = 100.0f;
	static constexpr float radius = 15.0f;
};

