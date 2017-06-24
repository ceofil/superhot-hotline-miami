#pragma once
#include <fstream>
#include "RectF.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Enemy.h"
#include "Soldier.h"


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
	
	class SoldierEntry
	{
	public:
		SoldierEntry() = default;
		SoldierEntry(Vec2 pos_in, int angle_in);
		void Serialize(std::ofstream& out) const;
		void Deserialize(std::ifstream& in);
		Vec2 GetPos() const;
		float GetAngle() const;
		RectF GetRect() const;
	private:
		Vec2 pos;
		int angle;
	};

public:
	void AddWallEntry( RectF wall );
	void RemoveWallEntry(Vec2 cursor);

	void AddEnemyEntry(Vec2 pos, float angle);
	void RemoveEnemyEntry(Vec2 cursor);

	void SetPlayerEntry(Vec2 pos, float angle);

	void Load( const char* filename_in );
	void Save( const char* filename_out );
	void Implement( RectF* walls, int& currNumberWalls, Enemy * enemies, int& currNumberEnemies, Soldier& player);
	void Draw(Graphics& gfx);
	void DrawPlayerEntry(Graphics& gfx);
private:
	static constexpr int maxNumberWalls = 50;
	WallEntry wallEntries[maxNumberWalls];
	int currNumber_WallEntries = 0;

	static constexpr int maxNumberEnemies = 100;
	SoldierEntry enemyEntries[maxNumberEnemies];
	int currNumber_EnemyEntries = 0;
	SoldierEntry playerEntry;

};