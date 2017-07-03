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
	out << left << " ";
	out << right << " ";
	out << top << " ";
	out << bottom << " ";
}

void Level::WallEntry::Deserialize(std::ifstream & in)
{
	int left, right, top, bottom;

	in >> left >> right >> top >> bottom;

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
	std::ifstream in(filename_in);
	playerEntry.Deserialize(in);
	in >> currNumber_WallEntries;
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		wallEntries[i].Deserialize(in);
	}
	in >> currNumber_EnemyEntries;
	for (int i = 0; i < currNumber_EnemyEntries; i++)
	{
		enemyEntries[i].Deserialize(in);
	}
}

void Level::Save(const char * filename_out)
{
	std::ofstream out(filename_out);
	playerEntry.Serialize(out);
	out << currNumber_WallEntries << " ";
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		wallEntries[i].Serialize(out);
	}

	out << currNumber_EnemyEntries << " ";
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

void Level::Clear()
{
	SetPlayerEntry(Vec2(600.0f, 700.0f), 90.0f);
	currNumber_WallEntries = 0;
	currNumber_EnemyEntries = 0;
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
	Soldier(playerEntry.GetPos(), Enemy::AngleToVec2(float(playerEntry.GetAngle())) ).Draw(gfx);
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

	out << x << " ";
	out << y << " ";
	out << angle << " ";
}

void Level::SoldierEntry::Deserialize(std::ifstream & in)
{
	int x, y;

	in >> x >> y >> angle;

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

