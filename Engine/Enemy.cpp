#include "Enemy.h"

Enemy::Enemy(Vec2 pos_in, float angle_in)
	:
	enemy(pos_in,AngleToVec2(angle_in)),
	angle(angle_in)
{
}

void Enemy::Set(Vec2 pos_in, float angle_in)
{
	startAngle = angle_in;
	enemy.Set(pos_in, AngleToVec2(angle_in));
}

void Enemy::Respawn()
{
	enemy.Respawn();
	angle = startAngle;
	triggered = false;
	ResetTrackingPoints();
}

void Enemy::Draw(Graphics & gfx)
{
	gfx.DrawCircle(enemy.GetPos(), enemy.GetRadius(), Color(255,150,150));
	gfx.DrawCircleStrokeOnly(enemy.GetPos(), enemy.GetRadius(), 1.0f, Color(255,50,50));
	gfx.DrawCircle(enemy.GetBulletSpawnPoint(), enemy.GetRadius()*0.25f, Colors::Red);
	gfx.DrawLine(enemy.GetPos(), enemy.GetBulletSpawnPoint(), Colors::White);
	

	for (int i = 0; i < indexTrackingPoints; i++)
	{
		gfx.DrawCircle(trackingPoints[i], 5.0f, Colors::Magenta);
		if (i < indexTrackingPoints - 1)
		{
			gfx.DrawLine(trackingPoints[i], trackingPoints[i+1], Colors::White);
		}
	}
	
}


void Enemy::Update(const Soldier & player, 
	const RectF walls[], int indexWalls,
	Bullet bullets[], int nBullets,
	Bullet otherBullets[], int nOtherBullets,
	Sound& bulletShotSound, 
	float dt)
{
	if ( triggered )
	{
		if (CanSee(player, walls, indexWalls))
		{
			ResetTrackingPoints();

			AddTrackingPoint(player.GetPos());

			float wantedAngle = Vec2ToAngle(  player.GetPos() -  enemy.GetPos());

			RotateToward(wantedAngle, dt);

			if (std::abs(wantedAngle - angle) < 1.0f)
			{
				enemy.Shoot(bullets, nBullets, bulletShotSound);
				TrackTarget(player, walls, indexWalls, dt * 0.5f);
			}
		}
		else
		{
			

			TrackTarget(player, walls, indexWalls, dt);
		}
	}
	else
	{
		if (CanSee(player, walls, indexWalls))
		{
			triggered = true;
			enemy.shootCooldownLeft = Soldier::shootCooldown;
			AddTrackingPoint(player.GetPos());
		}
	}
	if (addPointCooldown > 0.0f)
	{
		addPointCooldown -= dt;
	}
	enemy.HandleBullets(otherBullets, nOtherBullets);
	enemy.shootCooldownLeft -= dt;
	angle = Vec2ToAngle(enemy.GetDir());

}

void Enemy::TrackTarget(const Soldier& player, const RectF walls[], int indexWalls, float dt)
{
	if (addPointCooldown <= 0.0f)
	{
		AddTrackingPoint(player.GetPos());
	}
	if (Vec2(trackingPoints[0] - enemy.GetPos()).GetLength() > enemy.GetRadius())
	{
		const Vec2 dir_in = Vec2(trackingPoints[0] - enemy.GetPos()).GetNormalized();

		float wantedAngle = Vec2ToAngle(dir_in);
		RotateToward(wantedAngle, dt);

		if (std::abs(wantedAngle - angle) < 1.0f)
		{
			enemy.Move(AngleToVec2(angle), dt);
			for (int i = 1; i <= indexWalls; i++)
			{
				enemy.DoWallCollision(walls[i], AngleToVec2(angle), dt);
			}
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
		addPointCooldown = 0.5f;
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

void Enemy::ResetTrackingPoints()
{
	while (indexTrackingPoints > 0)
	{
		RemoveTrackingPoint();
	}
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
