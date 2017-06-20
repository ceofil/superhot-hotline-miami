#include "Enemy.h"

Enemy::Enemy(Vec2 pos_in, float angle_in)
	:
	enemy(pos_in,AngleToVec2(angle_in)),
	angle(angle_in)
{
}

void Enemy::Spawn(Vec2 pos_in, float angle_in)
{
	enemy.Spawn(pos_in, AngleToVec2(angle_in));
	angle = angle_in;
}

void Enemy::Draw(Graphics & gfx)
{
	enemy.Draw(gfx, Colors::Yellow);
	for (int i = 0; i < indexTrackingPoints; i++)
	{
		gfx.DrawCircle(trackingPoints[i], 5.0f, Colors::Magenta);
		if (i < indexTrackingPoints - 1)
		{
			gfx.DrawLine(trackingPoints[i], trackingPoints[i+1], Colors::White);
		}
	}
}


void Enemy::Update(const Soldier & player, const RectF walls[], int indexWalls, Bullet bullets[], int nBullets, float dt)
{
	if ( triggered )
	{
		if (CanSee(player, walls, indexWalls))
		{
			while (indexTrackingPoints > 0)
			{
				RemoveTrackingPoint();
			}
			AddTrackingPoint(player.GetPos());

			float wantedAngle = Vec2ToAngle(  player.GetPos() -  enemy.GetPos());


			RotateToward(wantedAngle, dt);
			//Shoot()
		}
		else
		{
			TrackTarget(walls, indexWalls, dt);
			if (addPointCooldown <= 0.0f)
			{
				AddTrackingPoint(player.GetPos());
			}
		}
	}
	else
	{
		if (CanSee(player, walls, indexWalls))
		{
			triggered = true;
			AddTrackingPoint(player.GetPos());
		}
	}
	if (addPointCooldown > 0.0f)
	{
		addPointCooldown -= dt;
	}
	enemy.HandleBullets(bullets, nBullets);

}

void Enemy::TrackTarget(const RectF walls[], int indexWalls, float dt)
{
	if (Vec2(trackingPoints[0] - enemy.GetPos()).GetLength() > enemy.GetRadius())
	{
		const Vec2 dir_in = Vec2(trackingPoints[0] - enemy.GetPos()).GetNormalized();

		enemy.SetDir(dir_in);
		enemy.Move(dir_in, dt);
		for (int i = 1; i <= indexWalls; i++)
		{
			enemy.DoWallCollision(walls[i], dir_in, dt);
		}
	}
	else
	{
		RemoveTrackingPoint();
	}
}

bool Enemy::CanSee(const Soldier & player, const RectF walls[], int indexWalls) const
{
	bool test = true;

	for (int i = 1; i <= indexWalls; i++)
	{
		if( Line(enemy.GetPos(), player.GetPos()).OverlappingWith_Rect(walls[i]) )
		{
			test = false;
		}
		
	}

	return test;
}

void Enemy::RotateToward(float wantedAngle, float dt)
{
	if (int((wantedAngle - angle) + 360) % 360 > 180)
	{
		angle -= rotationSpeed * dt;
	}
	else
	{
		angle += rotationSpeed * dt;
	}
	NormalizeAngle(angle);
	enemy.SetDir(AngleToVec2(angle));
}

Vec2 Enemy::GetPos() const
{
	return enemy.GetPos();
}

bool Enemy::IsAlive() const
{
	return enemy.IsAlive();
}

void Enemy::AddTrackingPoint(Vec2 tp)
{
	if(indexTrackingPoints < nTrackingPoints)
	{ 
		trackingPoints[++indexTrackingPoints] = tp;
		addPointCooldown = 0.2f;
	}
	
}

void Enemy::RemoveTrackingPoint()
{
	for (int i = 0; i < indexTrackingPoints; i++)
	{
		trackingPoints[i] = trackingPoints[i + 1];
	}
	indexTrackingPoints--;
}

Vec2 Enemy::AngleToVec2(float angle)
{
	const float cos = std::cos(angle * pi / 180.0f);
	const float sin = std::sin(angle * pi / 180.0f);
	return Vec2(cos, -sin); 
}

float Enemy::Vec2ToAngle(Vec2 vec)
{
	vec.Normalize();
	float angle = -asin(vec.y) * 180.0 / pi;

	if (vec.x > 0) return angle;
	else return 180.0f - angle;
}

void Enemy::NormalizeAngle(float & angle)
{
	while (std::abs(angle) >= 360.0f)
	{
		if (angle > 0.0f) angle -= 360.0f;
		else angle += 360.0f;
	}
}
