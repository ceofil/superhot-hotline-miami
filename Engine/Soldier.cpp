#include "Soldier.h"

Soldier::Soldier(Vec2 pos_in, Vec2 dir_in)
	:
	pos(pos_in),
	dir(dir_in)
{
	alive = true;
}

void Soldier::Set(Vec2 pos_in, Vec2 dir_in)
{
	startPos = pos_in;
	startDir = dir_in;
}

void Soldier::Respawn()
{
	alive = true;
	pos = startPos;
	dir = startDir;
}


void Soldier::Update(Keyboard & kbd, Mouse& mouse, 
	const RectF walls[], int currNumberWalls,
	Bullet bullets[], int nBullets,
	Bullet otherBullets[], int nOtherBullets, 
	float dt)
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
		active = true;
		pos += delta.GetNormalized() * speed * dt;
		for ( int i = 0; i <= currNumberWalls; i++)
		{
			DoWallCollision(walls[i], delta, dt);
		}
	}
	else
	{
		active = false;
	}
	dir = (mouse.GetPosVec2() - pos).GetNormalized();
	HandleBullets(otherBullets, nOtherBullets);
	shootCooldownLeft -= dt;
}

void Soldier::Draw(Graphics & gfx, Color c)
{
	gfx.DrawCircle(pos, radius, c);

	gfx.DrawLine(pos, GetBulletSpawnPoint(), c);
	gfx.DrawCircle(GetBulletSpawnPoint(), radius*0.25f, Colors::Red);

}

void Soldier::Shoot(Bullet bullets[], int nBullets, Sound& bulletShotSound)
{
	if (shootCooldownLeft <= 0.0f)
	{
		for (int i = 0; i < nBullets; i++)
		{
			if (bullets[i].IsSpawned() == false)
			{
				bullets[i].Spawn(GetBulletSpawnPoint(), dir);
				bulletShotSound.Play(1.0f, 0.2f);
				shootCooldownLeft = shootCooldown;
				break;
			}
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

void Soldier::HandleBullets(Bullet otherBullets[], int nOtherBullets)
{
	for (int i = 0; i < nOtherBullets; i++)
	{
		if (otherBullets[i].IsSpawned())
		{
			if (Vec2(otherBullets[i].GetPosition() - pos).GetLength() <= radius )
			{
				alive = false;
				otherBullets[i].Destroy();
			}
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
	return Vec2(pos + dir * radius * 1.5f);
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

bool Soldier::IsActive() const
{
	return active;
}
