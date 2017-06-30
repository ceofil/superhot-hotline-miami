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
	angle = angle_in;
	enemy.Set(pos_in, AngleToVec2(angle_in));
	enemy.SetSpeed(100.0f);
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
	gfx.DrawCircle(enemy.GetPos(), Soldier::GetRadius(), Color(255,150,150));
	gfx.DrawCircleStrokeOnly(enemy.GetPos(), Soldier::GetRadius(), 1.0f, Color(255,50,50));
	gfx.DrawCircle(enemy.GetBulletSpawnPoint(), Soldier::GetRadius()*0.25f, Colors::Red);
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
	const RectF walls[], int currNumberWalls,
	Bullet bullets[], int nBullets,
	Bullet otherBullets[], int nOtherBullets,
	Sound& bulletShotSound, 
	float dt)
{
	
	if (triggered)
	{
		if (CanSee(player, walls, currNumberWalls))
		{
			target = player.GetPos();
			ResetTrackingPoints();
			TrackTarget( walls,currNumberWalls, dt * 0.5f );
			enemy.Shoot( bullets, nBullets, bulletShotSound );
		}
		else
		{
			enemy.shootCooldownLeft = 0.5f;
			AddTrackingPoint(player.GetPos());
			target = trackingPoints[0];
			TrackTarget( walls, currNumberWalls, dt );
		}
	}
	else
	{
		if (CanSee(player, walls, currNumberWalls))
		{
			enemy.shootCooldownLeft = 0.75f;
			triggered = true;
			target = player.GetPos();
		}
	}

	enemy.HandleBullets(otherBullets, nOtherBullets);
	enemy.shootCooldownLeft -= dt;
	addPointCooldown -= dt;
	angle = Vec2ToAngle(enemy.GetDir());

}

void Enemy::TrackTarget( const RectF walls[], int currNumberWalls, float dt)
{
	float wantedAngle = Vec2ToAngle( target - enemy.GetPos() );

	if (std::abs( wantedAngle - angle ) < 1.0f )
	{
		if ((target - enemy.GetPos()).GetLengthSq() > enemy.GetRadiusSq())
		{
			enemy.Move(enemy.GetDir(), dt);
			for (int i = 0; i < currNumberWalls; i++)
			{
				enemy.DoWallCollision(walls[i], enemy.GetDir(), dt);
			}
		}
		else
		{
			RemoveTrackingPoint();
		}
	}
	else
	{
		RotateToward(wantedAngle, dt);
	}

}

bool Enemy::CanSee(const Soldier & player, const RectF walls[], int currNumberWalls) const
{
	for (int i = 0; i < currNumberWalls; i++)
	{
		if( Line(enemy.GetPos(), player.GetPos()).OverlappingWith_Rect(walls[i]) )
		{
			return false;
		}
	}
	return true;
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
	if (addPointCooldown <= 0.0f && indexTrackingPoints < nTrackingPoints) 
	{
		trackingPoints[indexTrackingPoints++] = tp;
		addPointCooldown = 0.5f;
	}
}

void Enemy::RemoveTrackingPoint()
{
	indexTrackingPoints--;
	for (int i = 0; i < indexTrackingPoints; i++)
	{
		trackingPoints[i] = trackingPoints[i + 1];
	}
}

void Enemy::ResetTrackingPoints()
{
	indexTrackingPoints = 0;
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
	float angle = -asin(vec.y) * 180.0f / pi;

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
