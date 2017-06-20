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
		const RectF walls[], int indexWalls, 
		Bullet bullets[], int nBullets,
		Bullet otherBullets[], int nOtherBullets,
		Sound& bulletShotSound, 
		float dt);
	void TrackTarget(const Soldier& player, const RectF walls[], int indexWalls, float dt);
	bool CanSee(const Soldier& player, const RectF walls[], int indexWalls) const;
	void RotateToward(float wantedAngle, float dt);


public:
	Vec2 GetPos() const;
	bool IsAlive() const;

private:
	Soldier enemy;
	bool triggered = false;

private:
	float addPointCooldown = 0.0f;
	static constexpr int nTrackingPoints = 85;
	int indexTrackingPoints = -1;
	Vec2 trackingPoints[nTrackingPoints];
	void AddTrackingPoint(Vec2 tp);
	void RemoveTrackingPoint();
	void ResetTrackingPoints();

private:
	static Vec2 AngleToVec2(float angle);
	static constexpr float pi = 3.14159265f;
	static constexpr float rotationSpeed = 175.0f;
	float angle;
	float startAngle;
public:
	static float Vec2ToAngle(Vec2 vec);
	static void NormalizeAngle(float & angle);
};