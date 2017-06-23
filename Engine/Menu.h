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
		 Soldier& player);
	void Draw(Graphics& gfx, Mouse& mouse, Text& txt);
	void HandleMousePressed(Mouse& mouse);
	void AddWall(Mouse& mouse);

private:

	Button start;
	Button save;
	Button implement;
	Button load;
	Button editor;
	Button addWall;
	Button back;

	Enemy* enemies;
	int maxNumberEnemies;
	int& currNumberEnemies;

	RectF* walls;
	int maxNumberWalls;
	int& currNumberWalls;
	int x1, x2, y1, y2;
	bool wallStarted = false;
	RectF rectBuffer;
	
	Soldier& player;

	Level level;

public:
	enum class GameState
	{
		firstMenu,
		levelEditor,
		gameStarted
	};
	enum class EditorState
	{
		nothing,
		addEnemy,
		addWall
	};
	GameState gameState = GameState::firstMenu;
	EditorState editorState = EditorState::nothing;
};