#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "RectF.h"
#include <chrono>
#include "Sound.h"

class Bullet
{
public:
	Bullet() = default;
	Bullet( Vec2 pos_in, Vec2 dir_in );
	void Draw( Graphics& gfx ) const;
	void Update(float dt, RectF walls[], int indexWalls, Sound& wallBounceSound);
	void Spawn(Vec2 pos_in, Vec2 dir_in);
	void Destroy();
public:
	void SetDirection(const Vec2& dir);
	RectF GetRect() const;
	Vec2 GetVelocity() const;
	Vec2 GetPosition() const;
	bool IsSpawned() const;
	static constexpr float radius = 2.5f;
private:
	RectF screen;
	bool spawned = false;
	float speed = 200.0f;
	Vec2 pos;
	Vec2 vel;
};