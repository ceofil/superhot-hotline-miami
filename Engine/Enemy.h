#pragma once
#include "Soldier.h"
#include "Graphics.h"
#include "Line.h"

class Enemy
{
public:
	Enemy() = default;
	Enemy(Vec2 pos, Vec2 dir);
	void Draw(Graphics& gfx);
	void Update(const Soldier& player, const RectF walls[], int indexWalls, float dt);
	void TrackTarget(const RectF walls[], int indexWalls, float dt);
	bool CanSee(const Soldier& player, const RectF walls[], int indexWalls);
	Vec2 GetPos() const;
private:
	Soldier enemy;
	bool triggered = false;
private:
	float addPointCooldown = 0.0f;
	static constexpr int nTrackingPoints = 15;
	int indexTrackingPoints = -1;
	Vec2 trackingPoints[nTrackingPoints];
	void AddTrackingPoint(Vec2 tp);
	void RemoveTrackingPoint();
};