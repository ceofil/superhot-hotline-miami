#include "Soldier.h"

Soldier::Soldier(Vec2 pos, Vec2 dir)
	:
	pos(pos),
	dir(dir)
{
}


void Soldier::Update(Keyboard & kbd, Mouse& mouse, RectF walls[], int indexWalls, float dt)
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

void Soldier::Draw(Graphics & gfx)
{
	gfx.DrawCircle(pos, radius, Colors::Green);

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
			Vec2 corner = { 0.0f, 0.0f };
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
	}
}



Vec2 Soldier::GetBulletSpawnPoint()
{
	return Vec2(pos + dir * radius * 2.5f);
}

RectF Soldier::GetRect()
{
	return RectF::FromCenter(pos, radius, radius);
}
