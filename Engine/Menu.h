#pragma once
#include "Button.h"
#include "Graphics.h"
#include "Enemy.h"
#include "Soldier.h"
#include "RectF.h"
#include "Level.h"

class Menu
{
public:
	Menu() = default;
	Menu(Enemy* enemies_in, int maxNumberEnemies_in, int& currNumberEnemies_in,
		 RectF* walls_in, int maxNumberWalls_in, int& currNumberWalls_in,
		 Soldier& player,
		 bool& gameIsStarted);
	void Draw(Graphics& gfx, Mouse& mouse, Text& txt);
	void HandleMousePressed(Mouse& mouse);
	void AddWall(Mouse& mouse);
private:

	Button start;
	Button save;
	Button implement;
	Button load;
	Button addWall;

	Enemy* enemies;
	int maxNumberEnemies;
	int& currNumberEnemies;

	RectF* walls;
	int maxNumberWalls;
	int& currNumberWalls;
	int x1, x2, y1, y2;
	bool wallStarted;
	RectF rectBuffer;
	
	Soldier& player;

	bool& gameIsStarted;

	static constexpr float wCenter = Graphics::ScreenWidth / 2;
	static constexpr float hCenter = Graphics::ScreenHeight / 2;
	static constexpr float width = 150.0f;

	Level level;
};