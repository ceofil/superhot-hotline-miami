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
	void Set(Vec2 pos_in, Vec2 dir_in);
	void Respawn();
	void Update(Keyboard& kbd, Mouse& mouse, const RectF walls[], int currNumberWalls, Bullet bullets[], int nBullets,
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
	static float GetRadius();
	bool IsAlive() const;
	bool IsActive() const;
	float shootCooldownLeft = 0.0f;
	static constexpr float shootCooldown = 0.95f;
	
private:
	bool active = false;
	bool alive = false;
	Vec2 pos;
	Vec2 dir;
	Vec2 startPos;
	Vec2 startDir;
	float speed = 200.0f;
	static constexpr float radius = 12.0f;
};

