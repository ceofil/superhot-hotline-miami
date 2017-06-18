#include "Bullet.h"

Bullet::Bullet(Vec2 pos_in, Vec2 dir_in)
	:
	pos(pos_in)
{
	SetDirection(dir_in);
}

void Bullet::Draw(Graphics & gfx) const
{
	gfx.DrawCircle(pos, radius, Colors::White);
	gfx.DrawRectPoints(screen, Colors::LightGray);
}

void Bullet::Update(float dt, RectF walls[], int indexWalls, Sound& wallBounceSound)
{
	pos += vel * dt;
	
	if ( !GetRect().IsContainedBy( RectF(0.0f, float(Graphics::ScreenWidth), 0.0f, float(Graphics::ScreenHeight - 35)) ) )
	{
		Destroy();
	}

	for (int i = 1; i <= indexWalls; i++)
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

Vec2 Bullet::GetVelocity() const
{
	return vel;
}

Vec2 Bullet::GetPosition() const
{
	return pos;
}

void Bullet::SetDirection( const Vec2 & dir )
{
	vel = dir.GetNormalized() * speed;
}


bool Bullet::IsSpawned() const
{
	return spawned;
}


void Bullet::Spawn(Vec2 pos_in, Vec2 dir_in)
{
	spawned = true;
	pos = pos_in;
	vel = dir_in * speed;
}

void Bullet::Destroy()
{
	spawned = false;
}
