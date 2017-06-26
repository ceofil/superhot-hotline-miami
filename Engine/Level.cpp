#include "Level.h"

Level::WallEntry::WallEntry(RectF rect_in)
	:
	rect(rect_in)
{
}

void Level::WallEntry::Serialize(std::ofstream & out) const
{
	int left = int(rect.left);
	int right = int(rect.right);
	int top = int(rect.top);
	int bottom = int(rect.bottom);
	out.write(reinterpret_cast<const char*>(&left), sizeof(left));
	out.write(reinterpret_cast<const char*>(&right), sizeof(right));
	out.write(reinterpret_cast<const char*>(&top), sizeof(top));
	out.write(reinterpret_cast<const char*>(&bottom), sizeof(bottom));
}

void Level::WallEntry::Deserialize(std::ifstream & in)
{
	int left, right, top, bottom;

	in.read(reinterpret_cast<char*>(&left), sizeof(left));
	in.read(reinterpret_cast<char*>(&right), sizeof(right));
	in.read(reinterpret_cast<char*>(&top), sizeof(top));
	in.read(reinterpret_cast<char*>(&bottom), sizeof(bottom));

	rect = RectF(float(left), float(right), float(top), float(bottom));
}

RectF Level::WallEntry::GetRect() const
{
	return rect;
}

void Level::AddWallEntry(RectF  wall)
{
	wallEntries[currNumber_WallEntries++] = wall;
}

void Level::RemoveWallEntry(Vec2 cursor)
{
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		if (wallEntries[i].GetRect().ContainsPoint(cursor))
		{
			for (int j = i; j + 1 < currNumber_WallEntries; j++)
			{
				wallEntries[j] = wallEntries[j + 1];
			}
			currNumber_WallEntries--;
			break;
		}
	}
}

void Level::AddEnemyEntry(Vec2 pos, float angle)
{
	enemyEntries[currNumber_EnemyEntries++] = SoldierEntry( pos,int(angle) );
}

void Level::RemoveEnemyEntry(Vec2 cursor)
{
	for (int i = 0; i < currNumber_EnemyEntries; i++)
	{
		if (enemyEntries[i].GetRect().ContainsPoint(cursor))
		{
			for (int j = i; j + 1 < currNumber_EnemyEntries; j++)
			{
				enemyEntries[j] = enemyEntries[j + 1];
			}
			currNumber_EnemyEntries--;
			break;
		}
	}
}

void Level::SetPlayerEntry(Vec2 pos, float angle)
{
	playerEntry = SoldierEntry( pos, int(angle) );
}

void Level::Load(const char * filename_in)
{
	std::ifstream in(filename_in, std::ios::binary);
	playerEntry.Deserialize(in);
	in.read(reinterpret_cast<char*>(&currNumber_WallEntries), sizeof(currNumber_WallEntries));
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		wallEntries[i].Deserialize(in);
	}
	in.read(reinterpret_cast<char*>(&currNumber_EnemyEntries), sizeof(currNumber_EnemyEntries));
	for (int i = 0; i < currNumber_EnemyEntries; i++)
	{
		enemyEntries[i].Deserialize(in);
	}
}

void Level::Save(const char * filename_out)
{
	std::ofstream out(filename_out, std::ios::binary);
	playerEntry.Serialize(out);
	out.write(reinterpret_cast<char*>(&currNumber_WallEntries), sizeof(currNumber_WallEntries));
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		wallEntries[i].Serialize(out);
	}

	out.write(reinterpret_cast<char*>(&currNumber_EnemyEntries), sizeof(currNumber_EnemyEntries));
	for (int i = 0; i < currNumber_EnemyEntries; i++)
	{
		enemyEntries[i].Serialize(out);
	}
}

void Level::Implement(RectF * walls, int& currNumberWalls, Enemy * enemies, int& currNumberEnemies, Soldier& player)
{
	player.Set(playerEntry.GetPos(), Enemy::AngleToVec2(float(playerEntry.GetAngle()) ));
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		walls[i] = wallEntries[i].GetRect();
	}
	currNumberWalls = currNumber_WallEntries;

	for (int i = 0; i < currNumber_EnemyEntries; i++)
	{
		enemies[i].Set( enemyEntries[i].GetPos(), float(enemyEntries[i].GetAngle()) );
	}
	currNumberEnemies = currNumber_EnemyEntries;
}

void Level::Draw(Graphics & gfx)
{
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		gfx.DrawRectPoints(wallEntries[i].GetRect(),Colors::LightGray);
	}
	for (int i = 0; i < currNumber_EnemyEntries; i++)
	{
		Enemy(enemyEntries[i].GetPos(), float(enemyEntries[i].GetAngle())).Draw(gfx);
	}
	Soldier(playerEntry.GetPos(), Enemy::AngleToVec2(float(playerEntry.GetAngle())) ).Draw(gfx,Color(100,150,255));
}

void Level::DrawPlayerEntry(Graphics & gfx)
{
	Soldier(playerEntry.GetPos(), Enemy::AngleToVec2(float(playerEntry.GetAngle()))).Draw(gfx, Color(100, 150, 255));
}

Level::SoldierEntry::SoldierEntry(Vec2 pos_in, int angle_in)
	:
	pos(pos_in),
	angle(angle_in)
{
}

void Level::SoldierEntry::Serialize(std::ofstream & out) const
{
	int x = int(pos.x);
	int y = int(pos.y);

	out.write(reinterpret_cast<const char*>(&x), sizeof(x));
	out.write(reinterpret_cast<const char*>(&y), sizeof(y));
	out.write(reinterpret_cast<const char*>(&angle), sizeof(angle));
}

void Level::SoldierEntry::Deserialize(std::ifstream & in)
{
	int x, y;

	in.read(reinterpret_cast<char*>(&x), sizeof(x));
	in.read(reinterpret_cast<char*>(&y), sizeof(y));
	in.read(reinterpret_cast<char*>(&angle), sizeof(angle));
	pos = Vec2(float(x), float(y));
}

Vec2 Level::SoldierEntry::GetPos() const
{
	return pos;
}

int Level::SoldierEntry::GetAngle() const
{
	return angle;
}

RectF Level::SoldierEntry::GetRect() const
{
	return RectF::FromCenter(pos, Soldier::GetRadius(), Soldier::GetRadius() );
}

