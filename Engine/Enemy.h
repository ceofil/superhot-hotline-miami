#pragma once
#include "Soldier.h"
#include "Graphics.h"
#include "Line.h"
#include "Bullet.h"

class Enemy
{
public:
	Enemy() = default;
	Enemy(Vec2 pos_in, Vec2 dir_in);
	void Spawn(Vec2 pos_in, Vec2 dir_in);
	void Draw(Graphics& gfx);
	void Update(const Soldier& player, const RectF walls[], int indexWalls, Bullet bullets[], int nBullets, float dt);
	void TrackTarget(const RectF walls[], int indexWalls, float dt);
	bool CanSee(const Soldier& player, const RectF walls[], int indexWalls) const;

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
};