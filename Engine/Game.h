/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Sound.h"
#include "FrameTimer.h"
#include "Soldier.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Line.h"
#include "Menu.h"
#include "Text.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(float dt);
	/********************************/
	/*  User Functions              */
	void UpdateBullets(float dt);
	void DrawBullets();
	void HandleInput();
	void DrawWalls();
	void UpdateEnemies(float dt);
	void DrawEnemies();

	void resetGame();
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	Sound bulletShotSound;
	Menu menu;
	Text txt;
	FrameTimer ft;


	Soldier player;

	static constexpr int nEnemies = 100;
	Enemy enemies[nEnemies];
	int currNumberEnemies = 0;

	static constexpr int nBullets = 10;
	Bullet playerBullets[nBullets];
	static constexpr int nBulletsForEnemies = 100;
	Bullet enemyBullets[nBulletsForEnemies];

	static constexpr int nWalls = 50;
	RectF walls[nWalls];
	int currNumberWalls = 0;

	
	
	/********************************/
};