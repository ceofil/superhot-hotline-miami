#include "Soldier.h"

Soldier::Soldier(Vec2 pos_in, Vec2 dir_in)
	:
	pos(pos_in),
	dir(dir_in)
{
	alive = true;
}

void Soldier::Spawn(Vec2 pos_in, Vec2 dir_in)
{
	pos = pos_in;
	dir = dir_in;
	alive = true;
}


void Soldier::Update(Keyboard & kbd, Mouse& mouse, const RectF walls[], int indexWalls, Bullet bullets[], int nBullets, float dt)
{
	
	
	bool movementHappened = false;
	Vec2 delta = { 0.0f,0.0f };
	if (kbd.KeyIsPressed(0x57))
	{
		delta.y = -1.0f;
		movementHappened = true;
	}
	else if (kbd.KeyIsPressed(0x53))
	{
		delta.y = 1.0f;
		movementHappened = true;
	}
	if (kbd.KeyIsPressed(0x41))
	{
		delta.x = -1.0f;
		movementHappened = true;
	}
	else if (kbd.KeyIsPressed(0x44))
	{
		delta.x = 1.0f;
		movementHappened = true;
	}

	if (movementHappened == true)
	{
		pos += delta.GetNormalized() * speed * dt;
		for ( int i = 1; i <= indexWalls; i++)
		{
			DoWallCollision(walls[i], delta, dt);
		}
	}
	dir = (mouse.GetPosVec2() - pos).GetNormalized();
}

void Soldier::Draw(Graphics & gfx, Color c)
{
	gfx.DrawCircle(pos, radius, c);

	Vec2 aim = pos + dir * radius * 2.5f;
	gfx.DrawCircle( aim, radius*0.25f, Colors::Red );
	gfx.DrawLine( pos, aim, Colors::White );
}

void Soldier::Shoot(Bullet bullets[], int nBullets, Sound& bulletShotSound)
{
	for (int i = 0; i < nBullets; i++)
	{
		if (bullets[i].IsSpawned() == false)
		{
			bullets[i].Spawn( GetBulletSpawnPoint(), dir );
			bulletShotSound.Play(1.0f, 0.2f);
			break;
		}
	}
}

void Soldier::DoWallCollision(const RectF & wall, Vec2 delta, float dt)
{
	RectF rect = GetRect();
	if (rect.IsOverlappingWith(wall))
	{
		if (pos.x > wall.left && pos.x < wall.right)
		{
			pos.y -= delta.y * speed * dt;
		}
		else if (pos.y > wall.top && pos.y < wall.bottom)
		{
			pos.x -= delta.x * speed * dt;
		}
		else
		{
			Vec2 corner;
			if (pos.y < wall.top) corner.y = wall.top;
			else corner.y = wall.bottom;
			if (pos.x < wall.left) corner.x = wall.left;
			else corner.x = wall.right;

			if (std::abs((pos - corner).x) > std::abs((pos - corner).y))
			{
				pos.x -= delta.x * speed * dt;
			}
			else
			{
				pos.y -= delta.y * speed * dt;
			}
			if (rect.IsOverlappingWith(wall))
			{
				pos -= delta * speed * dt;
			}
		}


		if( !(pos.x > wall.left + 5.0F && pos.x < wall.right - 5.0F) && !(pos.y > wall.top + 5.0F && pos.y < wall.bottom - 5.0F) )
		{
			Vec2 dif = ( pos - wall.GetCenter() ).GetNormalized();
			
			if (std::abs( dir.x ) > std::abs( dir.y ))
			{
				Move(Vec2(0.0f, dif.y), dt);
			}
			else
			{
				Move(Vec2(dif.x, 0.0f), dt);
			}
		}
	}
}

void Soldier::HandleBullets(Bullet bullets[], int nBullets)
{
	for (int i = 0; i < nBullets; i++)
	{
		if ( bullets[i].GetRect().IsOverlappingWith( GetRect() ) )
		{
			alive = false;
			bullets[i].Destroy();
		}
	}
}

void Soldier::Move(Vec2 dir, float dt)
{
	pos += dir * speed * dt;
}

void Soldier::SetDir(Vec2 dir_in)
{
	dir = dir_in;
}

void Soldier::SetSpeed(float speed_in)
{
	speed = speed_in;
}

void Soldier::Kill()
{
	alive = false;
}



Vec2 Soldier::GetBulletSpawnPoint() const
{
	return Vec2(pos + dir * radius * 2.5f);
}

RectF Soldier::GetRect() const
{
	return RectF::FromCenter(pos, radius, radius);
}

Vec2 Soldier::GetPos() const 
{
	return pos;
}

Vec2 Soldier::GetDir() const
{
	return dir;
}

float Soldier::GetRadius() const
{
	return radius;
}

bool Soldier::IsAlive() const
{
	return alive;
}
