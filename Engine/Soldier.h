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
	void Update(Keyboard& kbd, Mouse& mouse, const RectF walls[], int indexWalls, float dt);
	void Draw(Graphics& gfx, Color c);
	void Shoot(Bullet bullets[], int nBullets, Sound& bulletShotSound);
	void DoWallCollision(const RectF& wall, Vec2 delta, float dt);
	void Move(Vec2 dir, float dt);
	void SetDir(Vec2 dir_in);
	void SetSpeed(float speed_in);

public:
	Vec2 GetBulletSpawnPoint() const;
	RectF GetRect() const;
	Vec2 GetPos() const;
	Vec2 GetDir() const;
	float GetRadius() const;
	
private:
	Vec2 pos;
	Vec2 dir;
	float speed = 200.0f;
	static constexpr float radius = 15.0f;
};

