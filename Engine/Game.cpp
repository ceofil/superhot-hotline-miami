/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	bulletShotSound(L"bulletShot.wav"),
	menu(enemies, nEnemies, currNumberEnemies, walls, nWalls, currNumberWalls, player, gameIsStarted),
	txt(gfx, 0, 0, 1)
{
	
	/*
	walls[currNumberWalls++] = RectF(100.0f, 200.0f, 100.0f, 200.0f);
	walls[currNumberWalls++] = RectF(300.0f, 400.0f, 100.0f, 200.0f);
	walls[currNumberWalls++] = RectF(500.0f, 600.0f, 200.0f, 350.0f);
	walls[currNumberWalls++] = RectF(100.0f, 300.0f, 300.0f, 350.0f);

	menu.AddRect(walls[0]);
	menu.AddRect(walls[1]);
	menu.AddRect(walls[2]);
	menu.AddRect(walls[3]);
	*/

	enemies[currNumberEnemies++].Set(Vec2(150.0f, 450.0f), 180.0f);
	enemies[currNumberEnemies++].Set(Vec2(250.0f, 250.0f), 90.0f);

	enemies[currNumberEnemies++].Set(Vec2(350.0f, 300.0f), 120.0f);
	enemies[currNumberEnemies++].Set(Vec2(450.0f, 450.0f), 90.0f);
	enemies[currNumberEnemies++].Set(Vec2(550.0f, 450.0f), 120.0f);
	enemies[currNumberEnemies++].Set(Vec2(550.0f, 50.0f), 180.0f);
	enemies[currNumberEnemies++].Set(Vec2(250.0f, 150.0f), 90.0f);
	enemies[currNumberEnemies++].Set(Vec2(150.0f, 250.0f), 180.0f);
	

	player.Set(Vec2(50.0f, 50.0f), Vec2(0.0f, 0.0f));

	resetGame();
}

void Game::Go()
{
	gfx.BeginFrame();	

	float timeperiod = ft.Mark();
	while (timeperiod > 0.0f)
	{
		const float dt = std::min(0.0025f, timeperiod);
		UpdateModel(dt);
		timeperiod -= dt;
	}

	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel(float dt)
{
	if (player.IsActive() == false)
	{
		dt = dt / 5.0f;
	}
	if (player.IsAlive() == false)
	{
		resetGame();
	}

	if (gameIsStarted)
	{
		player.Update(wnd.kbd, wnd.mouse, walls, currNumberWalls, playerBullets, nBullets, enemyBullets, nBulletsForEnemies, dt);
		UpdateEnemies(dt);
		UpdateBullets(dt);
	}

	HandleInput();
}



void Game::ComposeFrame()
{
	if (!gameIsStarted)
	{
		menu.Draw(gfx, wnd.mouse, txt);
	}
	else
	{
		player.Draw(gfx, Color(100, 150, 255));
		DrawEnemies();
		DrawBullets();
		DrawWalls();
	}
}


void Game::UpdateBullets(float dt)
{
	for (int i = 0; i < nBullets; i++)
	{
		if ( playerBullets[i].IsSpawned() )
		{
			playerBullets[i].Update(dt, walls, currNumberWalls, bulletShotSound);
		}
	}
	for (int i = 0; i < nBulletsForEnemies; i++)
	{
		if (enemyBullets[i].IsSpawned())
		{
			enemyBullets[i].Update(dt, walls, currNumberWalls, bulletShotSound);
		}
	}
}

void Game::DrawBullets()
{
	for (int i = 0; i < nBullets; i++)
	{
		if ( playerBullets[i].IsSpawned() )
		{
			playerBullets[i].Draw(gfx);
		}
	}
	for (int i = 0; i < nBulletsForEnemies; i++)
	{
		if (enemyBullets[i].IsSpawned())
		{
			enemyBullets[i].Draw(gfx);
		}
	}
}

void Game::HandleInput()
{
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			if (gameIsStarted)
			{
				player.Shoot(playerBullets, nBullets, bulletShotSound);
			}
			else
			{
				menu.HandleMousePressed(wnd.mouse);
			}
		}
	}
	if (!wnd.kbd.KeyIsEmpty())
	{
		const auto e = wnd.kbd.ReadKey();
		if (e.IsRelease())
		{
			switch (e.GetCode())
			{
			case 0x52:
				resetGame();
				break;
			case VK_ESCAPE:
				gameIsStarted = false;
				break;
			}
		}
	}
}

void Game::DrawWalls()
{
	for (int i = 0; i <= currNumberWalls; i++)
	{
		gfx.DrawRectPoints( walls[i], Colors::LightGray );
	}
}

void Game::UpdateEnemies(float dt)
{
	for (int i = 0; i < currNumberEnemies; i++)
	{
		if (enemies[i].IsAlive())
		{
			enemies[i].Update(player, walls, currNumberWalls, enemyBullets, nBulletsForEnemies, playerBullets, nBullets, bulletShotSound, dt);
		}
	}
}

void Game::DrawEnemies()
{
	for (int i = 0; i < currNumberEnemies; i++)
	{
		if (enemies[i].IsAlive())
		{
			enemies[i].Draw(gfx);
		}
	}
}

void Game::resetGame()
{
	player.Respawn();
	for (int i = 0; i < currNumberEnemies; i++)
	{
		enemies[i].Respawn(); 
	}
	for (int i = 0; i < nBullets; i++)
	{
		if (playerBullets[i].IsSpawned())
		{
			playerBullets[i].Destroy();
		}
	}
	for (int i = 0; i < nBulletsForEnemies; i++)
	{
		if (enemyBullets[i].IsSpawned())
		{
			enemyBullets[i].Destroy();
		}
	}
}

void Game::HandleMenuInput()
{
	while (!wnd.mouse.IsEmpty())
	{
		const Mouse::Event e = wnd.mouse.Read();
		if (e.GetType() == Mouse::Event::Type::LPress)
		{
			player.Shoot(playerBullets, nBullets, bulletShotSound);
		}
	}
}

