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
	Soldier(Vec2 pos_in, Vec2 dir_in);
	void Spawn(Vec2 pos_in, Vec2 dir_in);
	void Update(Keyboard& kbd, Mouse& mouse, const RectF walls[], int indexWalls, Bullet bullets[], int nBullets,
		Bullet otherBullets[], int nOtherBullets, float dt);
	void Draw(Graphics& gfx, Color c);
	void Shoot(Bullet bullets[], int nBullets, Sound& bulletShotSound);
	void DoWallCollision(const RectF& wall, Vec2 delta, float dt);
	void HandleBullets(Bullet otherBullets[], int nOtherBullets);

public:
	void Move(Vec2 dir, float dt);
	void SetDir(Vec2 dir_in);
	void SetSpeed(float speed_in);
	void Kill();

public:
	Vec2 GetBulletSpawnPoint() const;
	RectF GetRect() const;
	Vec2 GetPos() const;
	Vec2 GetDir() const;
	float GetRadius() const;
	bool IsAlive() const;
	float shootCooldown = 0.0f;
	
private:
	bool alive = false;
	Vec2 pos;
	Vec2 dir;
	float speed = 50.0f;
	static constexpr float radius = 15.0f;
};

