#pragma once
#include "Button.h"
#include "Graphics.h"
#include "Enemy.h"
#include "Soldier.h"
#include "RectF.h"
#include "Level.h"
#include "Bullet.h"

class Menu
{
public:
	Menu() = default;
	Menu(Enemy* enemies_in, int maxNumberEnemies_in, int& currNumberEnemies_in,
		 RectF* walls_in, int maxNumberWalls_in, int& currNumberWalls_in,
		 Soldier& player_in,
		Bullet * playerBullets, int maxNumberPlayerBullets, Bullet * enemyBullets, int maxNumberEnemyBullets);
	void Draw(Graphics& gfx, Mouse& mouse, Text& txt);
	void HandleMousePressed(Mouse& mouse);
	void AddWall(Mouse& mouse);
	void RemoveWall(Mouse& mouse);
	void AddEnemy(Mouse& mouse);
	void RemoveEnemy(Mouse& mouse);
	void PlacePlayer(Mouse& mouse);
	void RestartGame();


private:
	static constexpr int buttonsPerRow = 5;
	static constexpr float buttonWidth = 150.0f;
	static constexpr float buttonHeight = 50.0f;
	static constexpr float wSpacing = float(Graphics::ScreenWidth - buttonWidth * 5) / 6.0f;
	Button levels[50];
	Vec2 GetButtonCenter(int level);

private:
	Button start;
	Button editor;
	Button restart;

	Button back;
	Button save;
	Button implement;

	Button addWall;
	Button removeWall;

	Button addEnemy;
	Button removeEnemy;
	Button placePlayer;

	Enemy* enemies;
	int maxNumberEnemies;
	int& currNumberEnemies;
	bool enemyPlaced = false;
	Vec2 posBuffer;
	float angleBuffer;

	RectF* walls;
	int maxNumberWalls;
	int& currNumberWalls;
	int x1, x2, y1, y2;
	bool wallStarted = false;
	RectF rectBuffer;
	
	Soldier& player;
	bool playerPlaced = false;

	Bullet* playerBullets;
	Bullet* enemyBullets;
	int maxNumberPlayerBullets;
	int maxNumberEnemyBullets;

	Level level;

public:
	enum class GameState
	{
		firstMenu,
		selectLevel,
		levelEditor,
		gameStarted,
		playerDied
	};
	enum class EditorState
	{
		selectLevel,
		nothing,
		addWall,
		removeWall,
		addEnemy,
		removeEnemy,
		placePlayer
	};
	GameState gameState = GameState::firstMenu;
	EditorState editorState = EditorState::nothing;
private:
	int currLevel;
	int NumberOfLevels = 5;
	char fileNameBuffer[50];
	void IntToChar(char * Dst, int number);
	void SetFileName(char* Dst, int number);
	void ChangeLevel(int wantedLevel);
};