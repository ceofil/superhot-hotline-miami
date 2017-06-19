#include "Enemy.h"

Enemy::Enemy(Vec2 pos, Vec2 dir)
	:
	enemy(pos,dir)
{
	enemy.SetSpeed(100.0f);
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

void Enemy::Update(const Soldier & player, const RectF walls[], int indexWalls, float dt)
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

			enemy.SetDir((player.GetPos() - enemy.GetPos()).GetNormalized() );
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
	if (indexTrackingPoints < 0)
	{
		triggered = false;
	}
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

bool Enemy::CanSee(const Soldier & player, const RectF walls[], int indexWalls)
{
	bool test = true;

	for (int i = 1; i <= indexWalls; i++)
	{
		
		if ( Line( enemy.GetPos(), player.GetPos() ).OverlappingWith_Rect( walls[i] ) )
		{
			test = false;
		}
		
	}

	return test;
}

Vec2 Enemy::GetPos() const
{
	return enemy.GetPos();
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
