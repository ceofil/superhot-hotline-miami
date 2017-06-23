#include "Bullet.h"

Bullet::Bullet(Vec2 pos_in, Vec2 dir_in)
	:
	pos(pos_in),
	dir(dir_in)
{
}

void Bullet::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(pos, radius, Colors::White);
	gfx.DrawLine(pos, pos - dir*(radius*5.0f), Colors::Red);
}

void Bullet::Update(float dt, RectF walls[], int currNumberWalls, Sound& wallBounceSound)
{
	pos += dir * speed * dt;
	
	if ( !GetRect().IsContainedBy( RectF(0.0f, float(Graphics::ScreenWidth), 0.0f, float(Graphics::ScreenHeight - 35)) ) )
	{
		Destroy();
	}

	for (int i = 0; i < currNumberWalls; i++)
	{
		if (GetRect().IsOverlappingWith(walls[i]))
		{
			Destroy();
		}
	}
}

RectF Bullet::GetRect() const
{
	return RectF::FromCenter( pos,radius,radius );
}

Vec2 Bullet::GetDir() const
{
	return dir;
}

Vec2 Bullet::GetPosition() const
{
	return pos;
}



bool Bullet::IsSpawned() const
{
	return spawned;
}


void Bullet::Spawn(Vec2 pos_in, Vec2 dir_in)
{
	spawned = true;
	pos = pos_in;
	dir = dir_in.GetNormalized();
}

void Bullet::Destroy()
{
	spawned = false;
}
