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

void Level::Load(const char * filename_in)
{
	std::ifstream in(filename_in, std::ios::binary);
	in.read(reinterpret_cast<char*>(&currNumber_WallEntries), sizeof(currNumber_WallEntries));
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		wallEntries[i].Deserialize(in);
	}
}

void Level::Save(const char * filename_out)
{
	std::ofstream out(filename_out, std::ios::binary);
	out.write(reinterpret_cast<char*>(&currNumber_WallEntries), sizeof(currNumber_WallEntries));
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		wallEntries[i].Serialize(out);
	}
}

void Level::Implement(RectF * walls, int& currNumberWalls)
{
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		walls[i] = wallEntries[i].GetRect();
	}
	currNumberWalls = currNumber_WallEntries;
}

void Level::Draw(Graphics & gfx)
{
	for (int i = 0; i < currNumber_WallEntries; i++)
	{
		gfx.DrawRectPoints(wallEntries[i].GetRect(),Colors::LightGray);
	}
}
