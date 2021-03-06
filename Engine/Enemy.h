#pragma once
#include "Soldier.h"
#include "Graphics.h"
#include "Line.h"
#include "Bullet.h"
#include "Sound.h"

class Enemy
{
public:
	Enemy() = default;
	Enemy(Vec2 pos_in, float angle_in);
	void Set(Vec2 pos_in, float angle_in);
	void Respawn();
	void Draw(Graphics& gfx);
	void Update(const Soldier& player, 
		const RectF walls[], int currNumberWalls,
		Bullet bullets[], int nBullets,
		Bullet otherBullets[], int nOtherBullets,
		Sound& bulletShotSound, 
		float dt);
	void TrackTarget( const RectF walls[], int currNumberWalls, float dt);
	bool CanSee(const Soldier& player, const RectF walls[], int currNumberWalls) const;
	void RotateToward(float wantedAngle, float dt);


public:
	Vec2 GetPos() const;
	bool IsAlive() const;

private:
	Soldier enemy;
	bool triggered = false;

private:
	Vec2 target;
private:
	float addPointCooldown = 0.0f;
	static constexpr int nTrackingPoints = 85;
	int indexTrackingPoints = 0;
	Vec2 trackingPoints[nTrackingPoints];
	void AddTrackingPoint(Vec2 tp);
	void RemoveTrackingPoint();
	void ResetTrackingPoints();

private:
	static constexpr float pi = 3.14159265f;
	static constexpr float rotationSpeed = 175.0f;
	float angle;
	float startAngle;
public:
	static Vec2 AngleToVec2(float angle);
	static float Vec2ToAngle(Vec2 vec);
	static void NormalizeAngle(float & angle);
	static float DotProduct(Vec2 a, Vec2 b);
};