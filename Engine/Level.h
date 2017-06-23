#pragma once
#include <fstream>
#include "RectF.h"

class Level
{
private:
	class WallEntry
	{
	public:
		WallEntry() = default;
		WallEntry(RectF rect_in);
		void Serialize(std::ofstream& out) const;
		void Deserialize(std::ifstream& in);
		RectF GetRect() const;
	private:
		RectF rect;
	};
public:
	void AddWallEntry( RectF wall );
	void Load( const char* filename_in );
	void Save( const char* filename_out );
	void Implement( RectF* walls, int& currNumberWalls);
private:
	static constexpr int maxNumberWalls = 50;
	WallEntry wallEntries[maxNumberWalls];
	int currNumber_WallEntries = 0;

};